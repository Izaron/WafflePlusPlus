#pragma once

#include <clang/AST/DeclCXX.h>

namespace Waffle::RestController {

struct StructData {
    const clang::CXXRecordDecl* Decl;

    // @brief mapping from method to httpType
    std::unordered_map<const clang::CXXMethodDecl*, std::string_view> Mapping; 
};
using StructDatas = std::vector<StructData>;

} // namespace Waffle::RestController
