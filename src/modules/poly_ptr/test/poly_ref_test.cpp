#include <gtest/gtest.h>
#include <numbers>
#include "structs/structs.poly_ptr.h"
#include "common.h"

using namespace Waffle;

namespace {

void MoveRobot(poly_ref<model::Robot> robot) {
    robot.GoTo(500, 400);               // {x, y} = {500, 400}
    robot.Forward(100.0);               // {x, y} = {600, 400}
    robot.Turn(-std::numbers::pi / 2);  // {x, y} = {600, 400}
    robot.Forward(70.0);                // {x, y} = {600, 330}
}

} // namespace

TEST(PolyPtr, PolyRefRobotAndStringer) {
    test::TestRobot testRobot;
    ASSERT_EQ(poly_ref<model::Stringer>{testRobot}.String(), "TestRobot{x = 0.000000, y = 0.000000}");
    MoveRobot(poly_ref<model::Robot>{testRobot});
    ASSERT_EQ(poly_ref<model::Stringer>{testRobot}.String(), "TestRobot{x = 600.000000, y = 330.000000}");
}
