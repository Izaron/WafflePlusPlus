#include <gtest/gtest.h>
#include "registry.h"

using namespace Waffle;

TEST(RegistryTest, AddModule) {
    for (int i = 0; i < 10; ++i) {
        ModuleRegistry::AddModule(Module{});
    }
    ASSERT_EQ(ModuleRegistry::GetModules().size(), 10);
    ModuleRegistry::ClearModules();
}

TEST(RegistryTest, RegistratorMacro) {
    REGISTER_MODULE(dummy, {"hello"}, nullptr);
    ASSERT_EQ(ModuleRegistry::GetModules().size(), 1);
    ModuleRegistry::ClearModules();
}
