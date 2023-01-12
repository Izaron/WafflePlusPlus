#include <gtest/gtest.h>
#include <numbers>
#include "structs/structs.poly_ptr.h"
#include "common.h"

using namespace Waffle;

namespace {

void MoveRobot(poly_obj<model::Robot>& robot) {
    robot.GoTo(500, 400);               // {x, y} = {500, 400}
    robot.Forward(100.0);               // {x, y} = {600, 400}
    robot.Turn(-std::numbers::pi / 2);  // {x, y} = {600, 400}
    robot.Forward(70.0);                // {x, y} = {600, 330}
}

} // namespace

TEST(PolyPtr, PolyObjRobot) {
    test::TestRobot testRobot;

    poly_obj<model::Robot> poly{std::move(testRobot)};
    MoveRobot(poly);
    ASSERT_EQ(poly.GetX(), 600);
    ASSERT_EQ(poly.GetY(), 330);
}

TEST(PolyPtr, PolyObjStringer) {
    test::TestRobot testRobot;
    testRobot.GoTo(123.4, 567.8);

    poly_obj<model::Stringer> poly{std::move(testRobot)};
    ASSERT_EQ(poly.String(), "TestRobot{x = 123.400000, y = 567.800000}");
}
