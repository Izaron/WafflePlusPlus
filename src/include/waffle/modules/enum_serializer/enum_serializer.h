#pragma once

#include <string_view>

namespace Waffle {

template<typename EnumType>
EnumType FromString(std::string_view value);

template<typename EnumType>
std::string_view ToString(EnumType value);

} // namespace Waffle
