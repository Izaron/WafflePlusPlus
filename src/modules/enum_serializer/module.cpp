#include "module.h"

namespace Waffle {

std::string_view EnumSerializerModule::Name() const {
    return "enum_serializer";
}

void EnumSerializerModule::Do(Context& /*context*/) const {
    // ???
}

REGISTER_MODULE(EnumSerializerModule);

} // namespace Waffle
