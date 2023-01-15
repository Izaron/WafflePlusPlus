#pragma once

#include <clang/AST/DeclCXX.h>

namespace Waffle::Signals {

struct StructData {
    const clang::CXXRecordDecl* Decl;
    std::vector<clang::CXXMethodDecl*> Signals;
    std::vector<clang::CXXMethodDecl*> Slots;
};

using StructDatas = std::vector<StructData>;

} // namespace Waffle::Signals
