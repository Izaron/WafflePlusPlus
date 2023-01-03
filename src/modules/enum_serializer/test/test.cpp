#include <gtest/gtest.h>

extern int kek;

TEST(EnumSerializer, CommentStyles) {
    ASSERT_EQ(kek, 1337);
}
