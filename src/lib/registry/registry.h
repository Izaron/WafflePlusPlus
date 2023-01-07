#pragma once

#include <functional>
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

struct Module {
    std::string_view Name;
    std::vector<std::string_view> Commands;
    std::function<void(Context&)> Func;
};

class ModuleRegistry {
public:
    static void AddModule(Module module);
    static const std::vector<Module>& GetModules();
    static void ClearModules();
};

struct ModuleRegistrator {
    ModuleRegistrator(Module&& module) {
        ModuleRegistry::AddModule(std::move(module));
    }
};

} // namespace Waffle

#define MACRO_CONCAT( x, y ) x##y

#define REGISTER_MODULE(Name, Commands, Func) \
static ::Waffle::ModuleRegistrator \
MACRO_CONCAT(ModuleRegistrator, Name)(Module{#Name, Commands, Func})
