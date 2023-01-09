#pragma once

#include "common.h"

namespace Waffle::JsonDump {

StructDecls Collect(clang::ASTContext& ctx);
std::vector<std::string_view> Commands();

} // namespace Waffle::JsonDump
