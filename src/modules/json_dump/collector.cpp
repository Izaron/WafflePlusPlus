#include "collector.h"

#include <unordered_set>

#include <lib/comment/comment.h>
#include <lib/string_util/string_util.h>

#include <clang/AST/RecursiveASTVisitor.h>

namespace Waffle::JsonDump {

namespace {

constexpr std::string_view COMMAND_JSONABLE = "jsonable";
constexpr std::string_view COMMAND_STRING_VALUE = "stringvalue";

class Collector : public clang::RecursiveASTVisitor<Collector> {
public:
    explicit Collector(clang::ASTContext& ctx) : Ctx_{ctx} {}

    bool VisitRecordDecl(clang::RecordDecl* decl) {
        if (ParseCommentData(Ctx_, *decl)->FindByName(COMMAND_JSONABLE)) {
            AddStructDecl(*decl);
        }
        return true;
    }

    StructDecls&& GetDecls() && {
        return std::move(Decls_);
    }

private:
    void AddStructDecl(const clang::RecordDecl& clangRecord) {
        if (UsedClangRecords_.contains(&clangRecord)) {
            return;
        }
        UsedClangRecords_.insert(&clangRecord);

        auto& structDecl = Decls_.emplace_back();
        structDecl.Decl = &clangRecord;

        for (const auto clangField : clangRecord.fields()) {
            const auto commentData = ParseCommentData(Ctx_, *clangField);

            auto& fieldDecl = structDecl.Fields.emplace_back();
            fieldDecl.VariableName = clangField->getNameAsString();
            if (const auto* command = commentData->FindByName(COMMAND_STRING_VALUE)) {
                fieldDecl.JsonName = command->Text;
            } else {
                fieldDecl.JsonName = fieldDecl.VariableName;
            }

            const auto* type = clangField->getType().getTypePtr()->getUnqualifiedDesugaredType();
            fieldDecl.TypeName = GetTypeName(*type);
            fieldDecl.IsRecordType = type->getAsRecordDecl() != nullptr && !JSON_STRING_TYPES.contains(fieldDecl.TypeName);

            // recursively add field's type
            if (JSON_ARRAY_TYPES.contains(fieldDecl.TypeName) || JSON_NULLABLE_TYPES.contains(fieldDecl.TypeName)) {
                if (const auto* clangRecord = TryGetTemplateArgClangRecord(*type)) {
                    AddStructDecl(*clangRecord);
                }
            } else if (fieldDecl.IsRecordType) {
                AddStructDecl(*type->getAsRecordDecl());
            }
        }
    }

    const clang::RecordDecl* TryGetTemplateArgClangRecord(const clang::Type& type) {
        const auto* recordDecl = type.getAsRecordDecl();
        if (!recordDecl) {
            return nullptr;
        }
        if (const auto* specDecl = llvm::dyn_cast_or_null<clang::ClassTemplateSpecializationDecl>(recordDecl)) {
            const auto& args = specDecl->getTemplateArgs();
            const auto& arg = args.get(0);
            if (arg.getKind() != clang::TemplateArgument::Type) {
                return nullptr;
            }
            if (const auto* type = arg.getAsType().getTypePtr(); type->isRecordType()) {
                const std::string typeName = GetTypeName(*type);
                if (JSON_STRING_TYPES.contains(typeName)) {
                    return nullptr;
                }
                if (JSON_ARRAY_TYPES.contains(typeName)) {
                    return TryGetTemplateArgClangRecord(*type);
                }
                return type->getAsRecordDecl();
            }
        }
        return nullptr;
    }

    std::string GetTypeName(const clang::Type& type) {
        if (const auto* typeDecl = type.getAsTagDecl()) {
            return StringUtil::QualifiedName(*typeDecl);
        }
        return "";
    }

private:
    clang::ASTContext& Ctx_;
    StructDecls Decls_;
    std::unordered_set<const clang::RecordDecl*> UsedClangRecords_;
};

} // namespace

StructDecls Collect(clang::ASTContext& ctx) {
    Collector collector{ctx};
    collector.TraverseDecl(ctx.getTranslationUnitDecl());
    return std::move(collector).GetDecls();
}

std::vector<std::string_view> Commands() {
    return {COMMAND_JSONABLE, COMMAND_STRING_VALUE};
}

} // namespace Waffle::JsonDump
