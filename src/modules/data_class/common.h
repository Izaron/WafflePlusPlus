#pragma once

#include <clang/AST/Decl.h>

namespace Waffle::DataClass {

struct ClassData {
    std::string Name;
    const clang::RecordDecl* Decl;
};
using ClassDatas = std::vector<ClassData>;

} // namespace Waffle::DataClass
