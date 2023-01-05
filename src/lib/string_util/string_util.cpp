#include "string_util.h"

#include <clang/AST/Decl.h>

namespace Waffle::StringUtil {

std::string QualifiedName(const clang::NamedDecl& decl) {
    std::string name;
    llvm::raw_string_ostream stream{name};
    decl.printQualifiedName(stream);
    return name;
}

} // namespace Waffle::StringUtil
