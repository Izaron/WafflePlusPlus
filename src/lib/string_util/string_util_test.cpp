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

TEST(StringUtilTest, InsertBeforeExtSimple) {
    constexpr std::string_view s = "foo.cpp";
    constexpr std::string_view extra = "extra";
    ASSERT_STREQ(InsertBeforeExt(s, extra).data(), "foo.extra.cpp");
}

TEST(StringUtilTest, InsertBeforeExtSlashesAndDots) {
    constexpr std::string_view s = "/a/b/c/foo.header.cpp";
    constexpr std::string_view extra = "long_word";
    ASSERT_STREQ(InsertBeforeExt(s, extra).data(), "/a/b/c/foo.long_word.cpp");
}

TEST(StringUtilTest, RemoveLastExtSimple) {
    constexpr std::string_view s = "foo.extra.cpp";
    ASSERT_STREQ(std::string{RemoveLastExt(s)}.c_str(), "foo.extra");
}

TEST(StringUtilTest, RemoveLastExtNoExt) {
    constexpr std::string_view s = "very_long_word";
    ASSERT_STREQ(std::string{RemoveLastExt(s)}.c_str(), "very_long_word");
}

TEST(StringUtilTest, SplitBySpaceSimple) {
    constexpr std::string_view s = " aaa  bbbb ccccc";
    const auto split = SplitBySpace(s);
    ASSERT_EQ(split.size(), 3);
    ASSERT_STREQ(std::string{split[0]}.c_str(), "aaa");
    ASSERT_STREQ(std::string{split[1]}.c_str(), "bbbb");
    ASSERT_STREQ(std::string{split[2]}.c_str(), "ccccc");
}

TEST(StringUtilTest, SplitBySpaceSinglePart) {
    constexpr std::string_view s = "aaa";
    const auto split = SplitBySpace(s);
    ASSERT_EQ(split.size(), 1);
    ASSERT_STREQ(std::string{split[0]}.c_str(), "aaa");
}

TEST(StringUtilTest, SplitByDelim) {
    constexpr std::string_view s = "/employees/{id}/info/{type}";
    const auto split = SplitByDelim(s, '/');
    ASSERT_EQ(split.size(), 4);
    ASSERT_STREQ(std::string{split[0]}.c_str(), "employees");
    ASSERT_STREQ(std::string{split[1]}.c_str(), "{id}");
    ASSERT_STREQ(std::string{split[2]}.c_str(), "info");
    ASSERT_STREQ(std::string{split[3]}.c_str(), "{type}");
}
