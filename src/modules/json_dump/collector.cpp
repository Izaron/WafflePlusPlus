#include "collector.h"

#include <lib/comment/comment.h>

#include <clang/AST/RecursiveASTVisitor.h>

namespace Waffle::JsonDump {

namespace {

constexpr std::string_view COMMAND_JSONABLE = "jsonable";

class StructDeclsCollector : public clang::RecursiveASTVisitor<StructDeclsCollector> {
public:
    explicit StructDeclsCollector(clang::ASTContext& ctx) : Ctx_{ctx} {}

    bool VisitRecordDecl(clang::RecordDecl* decl) {
        if (ParseCommentData(Ctx_, *decl)->FindByName(COMMAND_JSONABLE)) {
            Decls_.emplace_back(decl);
        }
        return true;
    }

    StructDecls&& GetDecls() && {
        return std::move(Decls_);
    }

private:
    clang::ASTContext& Ctx_;
    StructDecls Decls_;
};

} // namespace

StructDecls CollectStructDecls(clang::ASTContext& ctx) {
    StructDeclsCollector collector{ctx};
    collector.TraverseDecl(ctx.getTranslationUnitDecl());
    return std::move(collector).GetDecls();
}

std::vector<std::string_view> Commands() {
    return {COMMAND_JSONABLE};
}

} // namespace Waffle::JsonDump
