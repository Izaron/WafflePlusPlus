#pragma once

#include <clang/AST/Decl.h>

namespace Waffle::EnumSerializer {

struct EnumData {
    using StringValues = std::vector<std::string>;
    using EnumConstantAndStringValue = std::pair<const clang::EnumConstantDecl*, StringValues>;

    const clang::EnumDecl* Decl;
    std::vector<EnumConstantAndStringValue> Constants;
};
using EnumDatas = std::vector<EnumData>;

} // namespace Waffle::EnumSerializer
