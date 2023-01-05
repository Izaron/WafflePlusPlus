#pragma once

#include <string>

namespace clang {
    class NamedDecl;
} // namespace clang

namespace Waffle::StringUtil {

std::string QualifiedName(const clang::NamedDecl& decl);

} // namespace Waffle::StringUtil
