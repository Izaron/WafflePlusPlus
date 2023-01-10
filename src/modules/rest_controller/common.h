#pragma once

#include <clang/AST/DeclCXX.h>

namespace Waffle::RestController {

struct MethodData {
    std::string HttpMethod;
    std::string Mapping;
    const clang::CXXMethodDecl* MethodDecl;
};

struct StructData {
    const clang::CXXRecordDecl* Decl;
    std::vector<MethodData> MethodDatas;
};

using StructDatas = std::vector<StructData>;

} // namespace Waffle::RestController
