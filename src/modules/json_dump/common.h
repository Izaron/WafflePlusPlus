#pragma once

#include <unordered_set>

#include <clang/AST/Decl.h>

namespace Waffle::JsonDump {

inline const std::unordered_set<std::string> JSON_STRING_TYPES = {
    "std::basic_string", "std::basic_string_view",
};
inline const std::unordered_set<std::string> JSON_ARRAY_TYPES = {
    "std::vector", "std::deque", "std::list", "std::forward_list", "std::array",
};
inline const std::unordered_set<std::string> JSON_NULLABLE_TYPES = {
    "std::optional",
};

struct FieldDecl {
    bool IsRecordType;
    std::string TypeName;
    std::string VariableName;
    std::string JsonName;
};

struct StructDecl {
    const clang::RecordDecl* Decl;
    std::list<FieldDecl> Fields;
};

using StructDecls = std::list<StructDecl>;

} // namespace Waffle::JsonDump
