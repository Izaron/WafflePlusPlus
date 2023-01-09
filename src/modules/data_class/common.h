#pragma once

#include <unordered_set>

#include <clang/AST/Decl.h>

namespace Waffle::DataClass {

struct ClassData {
    std::string Name;
    const clang::RecordDecl* Decl;
    std::unordered_set<const clang::FieldDecl*> GetterOnlyFields;
};
using ClassDatas = std::vector<ClassData>;

} // namespace Waffle::DataClass
