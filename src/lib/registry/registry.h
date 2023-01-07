#pragma once

#include <span>
#include <vector>

#include <lib/file/file.h>

namespace clang {
    class ASTContext;
} // namespace clang

namespace Waffle {

struct Context {
    IFileManager& FileManager;
    std::string_view InFile;
    clang::ASTContext& AstContext;
};

class IModule {
public:
    virtual ~IModule() = default;
    virtual std::string_view Name() const = 0;
    virtual std::span<const std::string_view> Commands() const = 0;
    virtual void Do(Context& context) const = 0;
};

using IModulePtr = std::unique_ptr<IModule>;

class ModuleRegistry {
public:
    static void AddModule(IModulePtr module);
    static std::span<IModulePtr> GetModules();
};

template<typename T>
struct ModuleRegistrator {
    ModuleRegistrator() {
        ModuleRegistry::AddModule(std::make_unique<T>());
    }
};

} // namespace Waffle

#define MACRO_CONCAT( x, y ) x##y

#define REGISTER_MODULE(Type) \
static ::Waffle::ModuleRegistrator<Type> \
MACRO_CONCAT(ModuleRegistrator, Type);
