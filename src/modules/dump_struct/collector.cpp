#include "collector.h"

#include <lib/comment/comment.h>

#include <clang/AST/RecursiveASTVisitor.h>

namespace Waffle::DumpStruct {

namespace {

constexpr std::string_view COMMAND_DUMPABLE = "dumpable";

class StructDataCollector : public clang::RecursiveASTVisitor<StructDataCollector> {
public:
    explicit StructDataCollector(clang::ASTContext& ctx) : Ctx_{ctx} {}

    bool VisitRecordDecl(clang::RecordDecl* decl) {
        if (ParseCommentData(Ctx_, *decl)->FindByName(COMMAND_DUMPABLE)) {
            Datas_.emplace_back(decl);
        }
        return true;
    }

    StructDatas&& GetDatas() && {
        return std::move(Datas_);
    }

private:
    clang::ASTContext& Ctx_;
    StructDatas Datas_;
};

} // namespace

StructDatas CollectStructDatas(clang::ASTContext& ctx) {
    StructDataCollector collector{ctx};
    collector.TraverseDecl(ctx.getTranslationUnitDecl());
    return std::move(collector).GetDatas();
}

std::vector<std::string_view> Commands() {
    return {COMMAND_DUMPABLE};
}

} // namespace Waffle::DumpStruct
