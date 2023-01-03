#include <gtest/gtest.h>
#include "registry.h"

using namespace Waffle;

struct DummyModule : IModule {
    std::string_view Name() const override { return ""; }
    void Do(Context& /*context*/) const override {};
};

TEST(RegistryTest, AddModule) {
    for (int i = 0; i < 10; ++i) {
        IModulePtr dummy;
        ModuleRegistry::AddModule(std::move(dummy));
    }
    ASSERT_EQ(ModuleRegistry::GetModules().size(), 10);
}

TEST(RegistryTest, RegistratorMacro) {
    REGISTER_MODULE(DummyModule);
    ASSERT_EQ(ModuleRegistry::GetModules().size(), 1);
}
