#pragma once

#include "common.h"

namespace Waffle::EnumSerializer {

EnumDatas Collect(clang::ASTContext& ctx);
std::vector<std::string_view> Commands();

} // namespace Waffle::EnumSerializer
