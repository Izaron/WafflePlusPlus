#pragma once

#include <string>
#include <vector>

namespace clang {
    class NamedDecl;
} // namespace clang

namespace Waffle::StringUtil {

std::string QualifiedName(const clang::NamedDecl& decl);

// "/a/b/c/foo.cpp" -> "foo.cpp"
std::string_view AfterLastSlash(std::string_view s);

// "foo.cpp" -> "foo.extra.cpp"
std::string InsertBeforeExt(std::string_view s, std::string_view extra);

// " a  b c" -> ["a", "b", "c"]
std::vector<std::string_view> SplitBySpace(std::string_view s);

} // namespace Waffle::StringUtil
