#pragma once

#include <lib/file/i_file.h>

#include <memory>

namespace clang {
    class ASTContext;
} // namespace clang

namespace Waffle {

struct Context {
    IFileManager& FileMgr;
    std::string_view InFile;
    clang::ASTContext& AstContext;
};

class IModule {
public:
    virtual ~IModule() = default;
    virtual std::string_view Name() const = 0;
    virtual void Do(Context& context) const = 0;
};

using IModulePtr = std::unique_ptr<IModule>;

} // namespace Waffle
