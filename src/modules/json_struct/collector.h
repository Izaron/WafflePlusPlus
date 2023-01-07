#pragma once

#include "common.h"

#include <span>

namespace Waffle::JsonStruct {

StructDatas CollectStructDatas(clang::ASTContext& ctx);
std::span<const std::string_view> Commands();

} // namespace Waffle::JsonStruct
