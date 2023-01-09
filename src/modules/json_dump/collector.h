#pragma once

#include "common.h"

namespace Waffle::JsonDump {

StructDecls CollectStructDecls(clang::ASTContext& ctx);
std::vector<std::string_view> Commands();

} // namespace Waffle::JsonDump
