#include "collector.h"

#include <lib/comment/comment.h>
#include <lib/string_util/string_util.h>

#include <clang/AST/RecursiveASTVisitor.h>

namespace Waffle::DataClass {

namespace {

constexpr std::string_view COMMAND_DATA_CLASS = "dataclass";

class ClassDatasCollector : public clang::RecursiveASTVisitor<ClassDatasCollector> {
public:
    explicit ClassDatasCollector(clang::ASTContext& ctx) : Ctx_{ctx} {}

    bool VisitRecordDecl(clang::RecordDecl* decl) {
        if (const auto* command = ParseCommentData(Ctx_, *decl)->FindByName(COMMAND_DATA_CLASS)) {
            auto parts = StringUtil::SplitBySpace(command->Text);
            if (!parts.empty()) {
                Datas_.emplace_back(std::string{parts[0]}, decl);
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
    return {COMMAND_DATA_CLASS};
}

} // namespace Waffle::DataClass
