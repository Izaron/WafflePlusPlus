#pragma once

#include <memory>
#include <span>
#include <string>

namespace clang {
    class ASTContext;
    class Decl;
} // namespace clang

namespace Waffle {

struct CommentCommand {
    std::string Name;
    std::string Text;
};

class ICommentData {
public:
    virtual const CommentCommand* FindByName(std::string_view name) const = 0;
    virtual std::span<const CommentCommand> GetAllCommands() const = 0;
};

std::unique_ptr<ICommentData> ParseCommentData(const clang::ASTContext& astContext, const clang::Decl& decl);

} // namespace Waffle
