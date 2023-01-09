#include "collector.h"

#include <lib/comment/comment.h>
#include <lib/string_util/string_util.h>

#include <clang/AST/RecursiveASTVisitor.h>

namespace Waffle::DataClass {

namespace {

constexpr std::string_view COMMAND_DATA_CLASS = "dataclass";
constexpr std::string_view COMMAND_GETTER_ONLY = "getteronly";

class ClassDatasCollector : public clang::RecursiveASTVisitor<ClassDatasCollector> {
public:
    explicit ClassDatasCollector(clang::ASTContext& ctx) : Ctx_{ctx} {}

    bool VisitRecordDecl(clang::RecordDecl* decl) {
        if (const auto* command = ParseCommentData(Ctx_, *decl)->FindByName(COMMAND_DATA_CLASS)) {
            std::unordered_set<const clang::FieldDecl*> getterOnlyFieldDecls;
            for (const auto* fieldDecl : decl->fields()) {
                if (const auto* fieldCommand = ParseCommentData(Ctx_, *fieldDecl)->FindByName(COMMAND_GETTER_ONLY)) {
                    getterOnlyFieldDecls.insert(fieldDecl);
                }
            }

            auto parts = StringUtil::SplitBySpace(command->Text);
            if (!parts.empty()) {
                Datas_.emplace_back(std::string{parts[0]}, decl, std::move(getterOnlyFieldDecls));
            }
        }
        return true;
    }

    ClassDatas&& GetDatas() && {
        return std::move(Datas_);
    }

private:
    clang::ASTContext& Ctx_;
    ClassDatas Datas_;
};

} // namespace

ClassDatas Collect(clang::ASTContext& ctx) {
    ClassDatasCollector collector{ctx};
    collector.TraverseDecl(ctx.getTranslationUnitDecl());
    return std::move(collector).GetDatas();
}

std::vector<std::string_view> Commands() {
    return {COMMAND_DATA_CLASS, COMMAND_GETTER_ONLY};
}

} // namespace Waffle::DataClass
