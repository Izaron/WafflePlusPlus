#pragma once

#include "common.h"

#include <span>

namespace Waffle::JsonDump {

StructDatas CollectStructDatas(clang::ASTContext& ctx);
std::vector<std::string_view> Commands();

} // namespace Waffle::JsonDump
