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

// "foo.extra.cpp" -> "foo.extra"
std::string_view RemoveLastExt(std::string_view s);

// " a  b c" -> ["a", "b", "c"]
std::vector<std::string_view> SplitBySpace(std::string_view s);

// " a/b/c" -> ["a", "b", "c"]
std::vector<std::string_view> SplitByDelim(std::string_view s, char delim);

} // namespace Waffle::StringUtil
