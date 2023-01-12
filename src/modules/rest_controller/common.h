#pragma once

#include <clang/AST/DeclCXX.h>

namespace Waffle::RestController {

struct ParamData {
    const clang::ParmVarDecl* Decl;
    std::string Kind;
};

struct MethodData {
    std::string HttpMethod;
    std::string Mapping;
    const clang::CXXMethodDecl* Decl;
    std::vector<ParamData> ParamDatas;
};

struct StructData {
    const clang::CXXRecordDecl* Decl;
    std::vector<MethodData> MethodDatas;
};

using StructDatas = std::vector<StructData>;

} // namespace Waffle::RestController
