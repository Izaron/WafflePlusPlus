#pragma once

#include <nlohmann/json.hpp>

namespace Waffle {

template<typename T>
nlohmann::json ToJson(const T& value);

template<typename T>
T FromJson(const nlohmann::json& value);

} // namespace Waffle
