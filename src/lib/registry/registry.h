#pragma once

#include "module.h"

#include <span>
#include <vector>

namespace Waffle {

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
