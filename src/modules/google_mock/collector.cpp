#include "collector.h"

#include <lib/comment/comment.h>

#include <clang/AST/RecursiveASTVisitor.h>

namespace Waffle::GoogleMock {

namespace {

constexpr std::string_view COMMAND_GMOCK = "gmock";

class Collector : public clang::RecursiveASTVisitor<Collector> {
public:
    explicit Collector(clang::ASTContext& ctx) : Ctx_{ctx} {}

    bool VisitCXXRecordDecl(clang::CXXRecordDecl* decl) {
        if (ParseCommentData(Ctx_, *decl)->FindByName(COMMAND_GMOCK)) {
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

StructDecls Collect(clang::ASTContext& ctx) {
    Collector collector{ctx};
    collector.TraverseDecl(ctx.getTranslationUnitDecl());
    return std::move(collector).GetDecls();
}

std::vector<std::string_view> Commands() {
    return {COMMAND_GMOCK};
}

} // namespace Waffle::GoogleMock
