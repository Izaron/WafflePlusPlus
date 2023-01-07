#include <gtest/gtest.h>
#include <waffle/modules/dump_struct/dump_struct.h>
#include "simple_structs/simple_structs.h"

using namespace Waffle;

TEST(DumpStruct, SimpleStructTest1) {
    Test1 value;
    value.val1 = 1;
    value.val2 = 2;
    value.val3 = 3;
    value.val4 = 4;
    value.val5 = 5;
    value.val7.val1 = 71;
    value.val7.val2.val1 = "i'm a nested string";
    value.val7.val3 = 73;
    value.val8 = "very_cool_string";
    value.val9 = "string_views are also cool";
    value.val10 = reinterpret_cast<void*>(0xDEADF00D);
    value.val11 = reinterpret_cast<std::string*>(0xFEEDCAFE);

    constexpr std::string_view expected = R"(
val1 = 1
val2 = 2
val3 = 3
val4 = 4
val5 = 5
val6 = EmptyStruct
val7 = NestedStruct
    val1 = 71
    val2 = NestedNestedStruct
        val1 = "i'm a nested string"
    val3 = 73
val8 = "very_cool_string"
val9 = "string_views are also cool"
val10 = 0xdeadf00d
val11 = 0xfeedcafe
)";

    ASSERT_EQ("\n" + DumpStruct(value), expected);
}
