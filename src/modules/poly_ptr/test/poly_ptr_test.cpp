#include <gtest/gtest.h>
#include <numbers>
#include "structs/structs.poly_ptr.h"
#include "common.h"

using namespace Waffle;

namespace {

void MoveRobot(poly_ptr<model::Robot>& robot) {
    robot.GoTo(500, 400);               // {x, y} = {500, 400}
    robot.Forward(100.0);               // {x, y} = {600, 400}
    robot.Turn(-std::numbers::pi / 2);  // {x, y} = {600, 400}
    robot.Forward(70.0);                // {x, y} = {600, 330}
}

} // namespace

TEST(PolyPtr, PolyPtrRobot) {
    test::TestRobot testRobot;

    poly_ptr<model::Robot> poly{std::move(testRobot)};
    MoveRobot(poly);
    ASSERT_EQ(poly.GetX(), 600);
    ASSERT_EQ(poly.GetY(), 330);
}

TEST(PolyPtr, PolyPtrStringer) {
    test::TestRobot testRobot;
    testRobot.GoTo(123.4, 567.8);

    poly_ptr<model::Stringer> poly{std::move(testRobot)};
    ASSERT_EQ(poly.String(), "TestRobot{x = 123.400000, y = 567.800000}");
}
