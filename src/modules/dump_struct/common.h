#pragma once

#include <clang/AST/Decl.h>

namespace Waffle::DumpStruct {

using StructDatas = std::vector<const clang::RecordDecl*>;

} // namespace Waffle::DumpStruct
