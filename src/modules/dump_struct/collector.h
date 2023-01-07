#pragma once

#include "common.h"

#include <span>

namespace Waffle::DumpStruct {

StructDatas CollectStructDatas(clang::ASTContext& ctx);
std::vector<std::string_view> Commands();

} // namespace Waffle::DumpStruct
