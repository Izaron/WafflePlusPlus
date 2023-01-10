#include "printer.h"

#include <unordered_set>

#include <clang/AST/DeclTemplate.h>
#include <inja/inja.hpp>

#include <lib/file/file.h>
#include <lib/string_util/string_util.h>

namespace Waffle::JsonDump {

namespace {

#include "template.cpp.data"
const std::string TEMPLATE{(char*)template_cpp, template_cpp_len};

const std::unordered_set<std::string> JSON_STRING_TYPES = {
    "std::basic_string", "std::basic_string_view",
};
const std::unordered_set<std::string> JSON_ARRAY_TYPES = {
    "std::vector", "std::deque", "std::list", "std::forward_list", "std::array",
};
const std::unordered_set<std::string> JSON_NULLABLE_TYPES = {
    "std::optional",
};

class Printer {
public:
    explicit Printer(Context& ctx, const StructDecls& decls)
        : Ctx_{ctx}
    {
        for (const auto decl : decls) {
            DeclsQueue_.emplace(decl);
        }
    }

    void Print() {
        const std::string_view inFile = StringUtil::AfterLastSlash(Ctx_.InFile);
        const std::string outputFile = StringUtil::InsertBeforeExt(inFile, "json_dump");
        auto& printer = Ctx_.FileManager.GetOrCreateFilePrinter(outputFile);

        DataJson_["source_file"] = StringUtil::RemoveLastExt(inFile);

        while (!DeclsQueue_.empty()) {
            const auto decl = DeclsQueue_.front();
            DeclsQueue_.pop();
            if (!PrintedDecls_.insert(decl).second) {
                continue;
            }
            AddStructJson(decl);
        }
        std::reverse(DataJson_["structs"].begin(), DataJson_["structs"].end());

        inja::Environment env;
        env.set_trim_blocks(true);
        printer << env.render(TEMPLATE, DataJson_);
    }

private:
    void AddStructJson(const StructDecl decl) {
        auto& structJson = DataJson_["structs"].emplace_back();
        structJson["name"] = StringUtil::QualifiedName(*decl);

        auto& fieldsJson = structJson["fields"];
        for (const auto field : decl->fields()) {
            auto& fieldJson = fieldsJson.emplace_back();
            auto& funcSuffixJson = fieldJson["func_suffix"];
            fieldJson["name"] = field->getNameAsString();

            const auto* type = field->getType().getTypePtr()->getUnqualifiedDesugaredType();
            const std::string typeName = GetTypeName(*type);

            if (JSON_ARRAY_TYPES.contains(typeName)) {
                funcSuffixJson = "Array";
                if (const auto structDecl = TryGetTemplateArgStructDecl(*type)) {
                    DeclsQueue_.push(structDecl);
                }
            }
            else if (JSON_NULLABLE_TYPES.contains(typeName)) {
                funcSuffixJson = "Nullable";
                if (const auto structDecl = TryGetTemplateArgStructDecl(*type)) {
                    DeclsQueue_.push(structDecl);
                }
            }
            else if (const auto* recordDecl = type->getAsRecordDecl(); recordDecl && !JSON_STRING_TYPES.contains(typeName)) {
                funcSuffixJson = "Object";
                DeclsQueue_.push(recordDecl);
            }
            else {
                funcSuffixJson = "Primitive";
            }
        }
    }

    std::string GetTypeName(const clang::Type& type) {
        if (const auto* typeDecl = type.getAsTagDecl()) {
            return StringUtil::QualifiedName(*typeDecl);
        }
        return "";
    }

    StructDecl TryGetTemplateArgStructDecl(const clang::Type& type) {
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
                    return TryGetTemplateArgStructDecl(*type);
                }
                return type->getAsRecordDecl();
            }
        }
        return nullptr;
    }

private:
    Context& Ctx_;
    std::queue<StructDecl> DeclsQueue_;
    std::unordered_set<StructDecl> PrintedDecls_;
    inja::json DataJson_;
};

} // namespace

void Print(Context& ctx, const StructDecls& decls) {
    Printer printer{ctx, decls};
    printer.Print();
}

} // namespace Waffle::JsonDump
