#include "registry.h"

namespace Waffle {

static std::vector<Module> MODULES;

void ModuleRegistry::AddModule(Module module) {
    MODULES.emplace_back(std::move(module));
}

const std::vector<Module>& ModuleRegistry::GetModules() {
    return MODULES;
}

void ModuleRegistry::ClearModules() {
    MODULES.clear();
}

} // namespace Waffle
