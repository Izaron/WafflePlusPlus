#include "registry.h"

#include <vector>

namespace Waffle {

//static std::vector<IModulePtr> MODULES;

void ModuleRegistry::AddModule(IModulePtr module) {
    MODULES.emplace_back(std::move(module));
}

std::span<IModulePtr> ModuleRegistry::GetModules() {
    return MODULES;
}

} // namespace Waffle
