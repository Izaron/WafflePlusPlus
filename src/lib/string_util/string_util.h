#pragma once

#include <string>

namespace clang {
    class NamedDecl;
} // namespace clang

namespace Waffle::StringUtil {

std::string QualifiedName(const clang::NamedDecl& decl);
std::string_view AfterLastSlash(std::string_view s);

} // namespace Waffle::StringUtil
