#pragma once

#include <lib/module/module.h>

namespace Waffle {

class EnumSerializerModule : public IModule {
public:
    void Do(Context& context) override;
};

} // namespace Waffle
