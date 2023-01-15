#pragma once

#include <clang/AST/DeclCXX.h>

namespace Waffle::Signals {

struct StructData {
    const clang::CXXRecordDecl* Decl;
    std::vector<clang::CXXMethodDecl*> Signals;
};

using StructDatas = std::vector<StructData>;

} // namespace Waffle::Signals
