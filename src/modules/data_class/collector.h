#pragma once

#include "common.h"

namespace Waffle::DataClass {

ClassDatas Collect(clang::ASTContext& ctx);
std::vector<std::string_view> Commands();

} // namespace Waffle::DataClass
