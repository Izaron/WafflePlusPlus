#include "string_util.h"

#include <clang/AST/Decl.h>

namespace Waffle::StringUtil {

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

} // namespace Waffle::StringUtil
