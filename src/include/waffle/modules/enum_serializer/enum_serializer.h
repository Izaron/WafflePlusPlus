#pragma once

#include <span>
#include <string_view>

namespace Waffle {

template<typename EnumType>
EnumType FromString(std::string_view value);

template<typename EnumType>
EnumType FromStringOrDefault(std::string_view value, EnumType defaultResult);

template<typename EnumType>
std::string_view ToString(EnumType value);

template<typename EnumType>
std::span<const EnumType> GetAllEnumValues();

} // namespace Waffle
