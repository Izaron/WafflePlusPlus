#include "string_util.h"

#include <clang/AST/Decl.h>

namespace Waffle::StringUtil {

namespace {

template<typename Func>
std::vector<std::string_view> Split(std::string_view s, Func IsDelim) {
    std::vector<std::string_view> result;
    int first_non_delim = s.size();
    int last_non_delim = -1;
    for (int i = 0; i < s.size(); ++i) {
        if (IsDelim(s[i])) {
            if (first_non_delim != s.size()) {
                result.push_back(s.substr(first_non_delim, last_non_delim - first_non_delim + 1));
            }
            first_non_delim = s.size();
        } else {
            first_non_delim = std::min(first_non_delim, i);
            last_non_delim = i;
        }
    }
    if (first_non_delim != s.size()) {
        result.push_back(s.substr(first_non_delim, last_non_delim - first_non_delim + 1));
    }
    return result;
}

} // namespace

std::string QualifiedName(const clang::NamedDecl& decl) {
    std::string name;
    llvm::raw_string_ostream stream{name};
    decl.printQualifiedName(stream);
    return name;
}

std::string_view AfterLastSlash(std::string_view s) {
    if (const auto pos = s.find_last_of('/'); pos != std::string_view::npos) {
        s = s.substr(pos + 1);
    }
    return s;
}

std::string InsertBeforeExt(std::string_view s, std::string_view extra) {
    if (const auto lastPos = s.find_last_of('.'); lastPos != std::string_view::npos) {
        if (const auto prevLastPos = s.find_last_of('.', lastPos - 1); prevLastPos != std::string_view::npos) {
            return std::string{s.substr(0, prevLastPos)} + "." + std::string{extra} + std::string{s.substr(lastPos)};
        }
        return std::string{s.substr(0, lastPos)} + "." + std::string{extra} + std::string{s.substr(lastPos)};
    }
    return std::string{s};
}

std::string_view RemoveLastExt(std::string_view s) {
    if (const auto pos = s.find_last_of('.'); pos != std::string_view::npos) {
        return s.substr(0, pos);
    }
    return s;
}

std::vector<std::string_view> SplitBySpace(std::string_view s) {
    return Split(s, [](char c) { return std::isspace(c); });
}

std::vector<std::string_view> SplitByDelim(std::string_view s, char delim) {
    return Split(s, [delim](char c) { return c == delim; });
}

} // namespace Waffle::StringUtil
