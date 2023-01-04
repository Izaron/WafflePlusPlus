#include <gtest/gtest.h>
#include "comment.h"

#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Tooling/Tooling.h>

using namespace Waffle;

namespace {

const std::vector<std::string> ARGS = {
    "-fparse-all-comments",
    "-fcomment-block-commands=serializable,stringvalue",
};

// Parse comment from any VarDecl
std::unique_ptr<ICommentData> ParseCommentDataFromVarDecl(const clang::ASTContext& ctx) {
    struct Visitor : clang::RecursiveASTVisitor<Visitor> {
        Visitor(const clang::ASTContext& ctx) : Ctx{ctx} {}

        bool VisitVarDecl(clang::VarDecl* decl) {
            CommentData = ParseCommentData(Ctx, *decl);
            return true;
        }

        const clang::ASTContext& Ctx;
        std::unique_ptr<ICommentData> CommentData;
    } visitor{ctx};
    visitor.TraverseTranslationUnitDecl(ctx.getTranslationUnitDecl());

    return std::move(visitor.CommentData);
}

} // namespace

TEST(CommentTest, Simple) {
    constexpr std::string_view code = R"(
    /// @brief just `foo` value, nothing special
    /// \serializable
    /// @stringvalue FOO Foo foo
    int foo = 12345;
)";

    auto unit = clang::tooling::buildASTFromCodeWithArgs(code, ARGS);
    auto commentData = ParseCommentDataFromVarDecl(unit->getASTContext());

    // check size
    auto allCommands = commentData->GetAllCommands();
    ASSERT_EQ(allCommands.size(), 3);

    // check missing commands
    ASSERT_EQ(commentData->FindByName("author"), nullptr);
    ASSERT_EQ(commentData->FindByName("wrongname"), nullptr);

    // check existing commands
    auto& brief = *commentData->FindByName("brief");
    ASSERT_STREQ(brief.Name.c_str(), "brief");
    ASSERT_STREQ(brief.Text.c_str(), " just `foo` value, nothing special ");

    auto& serializable = *commentData->FindByName("serializable");
    ASSERT_STREQ(serializable.Name.c_str(), "serializable");
    ASSERT_STREQ(serializable.Text.c_str(), " ");

    auto& stringvalue = *commentData->FindByName("stringvalue");
    ASSERT_STREQ(stringvalue.Name.c_str(), "stringvalue");
    ASSERT_STREQ(stringvalue.Text.c_str(), " FOO Foo foo");
}
