#pragma once

#include "common.h"

namespace Waffle::GoogleMock {

StructDecls Collect(clang::ASTContext& ctx);
std::vector<std::string_view> Commands();

} // namespace Waffle::GoogleMock
