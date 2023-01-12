#pragma once

#include <clang/AST/Decl.h>

namespace Waffle::PolyPtr {

using StructDecl = const clang::CXXRecordDecl*;
using StructDecls = std::vector<StructDecl>;

} // namespace Waffle::PolyPtr
