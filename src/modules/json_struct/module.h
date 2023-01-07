#pragma once

#include <lib/module/module.h>

namespace Waffle {

class JsonStructModule : public IModule {
public:
    std::string_view Name() const override;
    std::span<const std::string_view> Commands() const override;
    void Do(Context& context) const override;
};

} // namespace Waffle
