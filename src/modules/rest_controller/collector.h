#pragma once

#include "common.h"

namespace Waffle::RestController {

StructDatas Collect(clang::ASTContext& ctx);
std::vector<std::string_view> Commands();

} // namespace Waffle::RestController
