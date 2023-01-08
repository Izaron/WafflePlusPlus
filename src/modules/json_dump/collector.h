#pragma once

#include "common.h"

#include <span>

namespace Waffle::JsonDump {

StructDecls CollectStructDecls(clang::ASTContext& ctx);
std::vector<std::string_view> Commands();

} // namespace Waffle::JsonDump
