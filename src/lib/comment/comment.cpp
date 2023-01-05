#include "comment.h"

#include <clang/AST/ASTContext.h>
#include <clang/AST/Comment.h>

#include <sstream>
#include <vector>

namespace Waffle {

namespace {

class CommentData : public ICommentData {
public:
    CommentData(const clang::ASTContext& astContext, const clang::Decl& decl)
        : AstContext_{astContext}
        , Decl_{decl}
    {}

    const CommentCommand* FindByName(std::string_view name) const override {
        for (const auto& command : Commands_) {
            if (command.Name == name) {
                return &command;
            }
        }
        return nullptr;
    }

    std::span<const CommentCommand> GetAllCommands() const override {
        return Commands_;
    }

    void Parse() {
        const auto* comment = AstContext_.getCommentForDecl(&Decl_, /*PP=*/nullptr);
        if (!comment) {
            return;
        }
        for (const auto* block : comment->getBlocks()) {
            ParseBlockContent(*block);
        }
    }

private:
    void ParseBlockContent(const clang::comments::BlockContentComment& content) {
        if (const auto* command = clang::dyn_cast_or_null<clang::comments::BlockCommandComment>(&content)) {
            ParseBlockCommand(*command);
        }
    }

    void ParseBlockCommand(const clang::comments::BlockCommandComment& command) {
        auto name = std::string{command.getCommandName(AstContext_.getCommentCommandTraits())};
        std::string text;
        if (const auto* paragraph = command.getParagraph()) {
            text = ParseParagraph(*paragraph);
        }
        Commands_.emplace_back(std::move(name), std::move(text));
    }

    std::string ParseParagraph(const clang::comments::ParagraphComment& paragraph) {
        std::stringstream ss;
        for (const auto* child = paragraph.child_begin(); child != paragraph.child_end(); ++child) {
            if (const auto* textComment = clang::dyn_cast_or_null<clang::comments::TextComment>(*child)) {
                ss << std::string{textComment->getText()};
            }
        }
        return ss.str();
    }

private:
    const clang::ASTContext& AstContext_;
    const clang::Decl& Decl_;
    std::vector<CommentCommand> Commands_;
};

} // namespace

std::unique_ptr<ICommentData> ParseCommentData(const clang::ASTContext& astContext, const clang::Decl& decl) {
    auto commentData = std::make_unique<CommentData>(astContext, decl);
    commentData->Parse();
    return commentData;
}

} // namespace Waffle
