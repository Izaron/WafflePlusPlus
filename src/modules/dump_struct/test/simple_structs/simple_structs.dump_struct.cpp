// Generated by the Waffle++ code generator. DO NOT EDIT!
// source: simple_structs.h

#include <sstream>

#include <waffle/modules/dump_struct/dump_struct.h>

#include "simple_structs.h"

namespace Waffle {

template<>
std::string DumpStruct(const Test0& value) {
    std::stringstream ss;
    ss << "{\n";
    ss << "}\n";
    return ss.str();
}

template<>
std::string DumpStruct(const Test1& value) {
    std::stringstream ss;
    ss << "{\n";
    ss << "    " << "val1 = " << value.val1 << "\n";
    ss << "    " << "val2 = " << value.val2 << "\n";
    ss << "    " << "val3 = " << value.val3 << "\n";
    ss << "    " << "val4 = " << value.val4 << "\n";
    ss << "    " << "val5 = " << value.val5 << "\n";
    ss << "    " << "val8 = \"" << value.val8 << "\"\n";
    ss << "    " << "val9 = \"" << value.val9 << "\"\n";
    ss << "    " << "val10 = " << value.val10 << "\n";
    ss << "    " << "val11 = " << value.val11 << "\n";
    ss << "}\n";
    return ss.str();
}

} // namespace Waffle