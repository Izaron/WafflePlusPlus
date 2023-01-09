#pragma once

#include <clang/AST/Decl.h>

namespace Waffle::GoogleMock {

using StructDecl = const clang::RecordDecl*;
using StructDecls = std::vector<StructDecl>;

} // namespace Waffle::GoogleMock
