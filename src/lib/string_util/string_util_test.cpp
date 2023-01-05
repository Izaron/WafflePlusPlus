#include <gtest/gtest.h>
#include "string_util.h"

using namespace Waffle::StringUtil;

TEST(StringUtilTest, AfterLastSlashSimple) {
    constexpr std::string_view s = "/home/me/some/folder/inside/cool_file.cpp";
    ASSERT_STREQ(AfterLastSlash(s).data(), "cool_file.cpp");
}

TEST(StringUtilTest, AfterLastSlashNoSlash) {
    constexpr std::string_view s = "cool_file.cpp";
    ASSERT_STREQ(AfterLastSlash(s).data(), "cool_file.cpp");
}
