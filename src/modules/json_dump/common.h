#pragma once

#include <clang/AST/Decl.h>

namespace Waffle::JsonDump {

using StructDecl = const clang::RecordDecl*;
using StructDecls = std::vector<StructDecl>;

} // namespace Waffle::JsonDump
