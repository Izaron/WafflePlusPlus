// Generated by the Waffle++ code generator. DO NOT EDIT!
// source: {{ source_file }}

#include <gmock/gmock.h>

#include "{{ source_file }}"

namespace Waffle {

## for struct in structs
class Mock{{ struct.name }} : public {{ struct.qualified_name }} {
public:
## for method in struct.methods
    MOCK_METHOD({{ method.return_type }}, {{ method.name }}, ({{ method.signature }}), ({{ method.qualifiers }}));
## endfor
};

## endfor
} // namespace Waffle
