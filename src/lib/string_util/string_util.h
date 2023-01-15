#pragma once

#include <string>
#include <vector>

namespace clang {
    class NamedDecl;
    class CXXMethodDecl;
} // namespace clang

namespace Waffle::StringUtil {

std::string QualifiedName(const clang::NamedDecl& decl);

// void foo(int x, double y, std::string z) -> "int x, double y, std::string z"
std::string GetSignature(const clang::CXXMethodDecl& methodDecl);

// void foo(int x, double y, std::string z) -> "x, y, z"
std::string JoinArgs(const clang::CXXMethodDecl& methodDecl);

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

// " a/b=c?d" -> ["a", "b", "c", "d"]
std::vector<std::string_view> SplitByDelims(std::string_view s, std::string_view delims);

} // namespace Waffle::StringUtil
