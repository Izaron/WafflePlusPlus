#pragma once

#include <lib/module/registry.h>

namespace Waffle {

class EnumSerializerModule : public IModule {
public:
    std::string_view Name() const override;
    void Do(Context& context) const override;
};

} // namespace Waffle
