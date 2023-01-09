#include <gtest/gtest.h>
#include "turtle/turtle.gmock.h"

using namespace Waffle;

using ::testing::AtLeast;

TEST(GoogleMock, Turtle) {
    MockTurtle turtle;
    EXPECT_CALL(turtle, PenDown()).Times(AtLeast(1));
    turtle.PenDown();
}
