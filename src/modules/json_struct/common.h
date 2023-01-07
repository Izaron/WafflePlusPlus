#pragma once

#include <clang/AST/Decl.h>

namespace Waffle::JsonStruct {

using StructDatas = std::vector<const clang::RecordDecl*>;

} // namespace Waffle::JsonStruct
