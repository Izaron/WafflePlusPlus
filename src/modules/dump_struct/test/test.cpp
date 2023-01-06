#include <gtest/gtest.h>
#include <waffle/modules/dump_struct/dump_struct.h>
#include "simple_structs/simple_structs.h"

using namespace Waffle;

TEST(DumpStruct, SimpleStructTest1) {
    Test1 value;
    value.aaa = 1;
    value.bbb = 2;
    value.ccc = 3;
    value.ddd = 4;
    value.eee = 5;

    constexpr std::string_view expected = R"(
aaa = 1
bbb = 2
ccc = 3
ddd = 4
eee = 5
)";

    ASSERT_EQ("\n" + DumpStruct(value), expected);
}
