#pragma once

#include <clang/AST/Decl.h>

namespace Waffle::JsonDump {

using StructDatas = std::vector<const clang::RecordDecl*>;

} // namespace Waffle::JsonDump
