#pragma once

#include <lib/registry/module.h>

namespace Waffle {

class DumpStructModule : public IModule {
public:
    std::string_view Name() const override;
    std::span<const std::string_view> Commands() const override;
    void Do(Context& context) const override;
};

} // namespace Waffle
