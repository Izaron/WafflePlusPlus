#pragma once

#include "common.h"

#include <span>

namespace Waffle::EnumSerializer {

EnumDatas CollectEnumDatas(clang::ASTContext& ctx);
std::span<const std::string_view> Commands();

} // namespace Waffle::EnumSerializer
