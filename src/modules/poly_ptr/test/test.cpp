#include <gtest/gtest.h>
#include <cmath>
#include <numbers>
#include "structs/structs.poly_ptr.h"

using namespace Waffle;

namespace {

class CoolRobot {
public:
    void Forward(double distance) {
        x_ += std::cos(angle_) * distance;
        y_ += std::sin(angle_) * distance;
    }

    void Turn(double degrees) {
        angle_ += degrees;
    };

    void GoTo(double x, double y) {
        x_ = x;
        y_ = y;
    }

    double GetX() const {
        return x_;
    };

    double GetY() const {
        return y_;
    };

    std::string String() const {
        return "CoolRobot{x = " + std::to_string(x_) + ", y = " + std::to_string(y_) + "}";
    }

private:
    double angle_ = 0;
    double x_ = 0;
    double y_ = 0;
};

void MoveRobot(poly_ptr<model::Robot>& robot) {
    robot.GoTo(500, 400);               // {x, y} = {500, 400}
    robot.Forward(100.0);               // {x, y} = {600, 400}
    robot.Turn(-std::numbers::pi / 2);  // {x, y} = {600, 400}
    robot.Forward(70.0);                // {x, y} = {600, 330}
}

std::string GetString(const poly_ptr<model::Stringer>& stringer) {
    return "you got string: " + stringer.String();
}

} // namespace

TEST(PolyPtr, PolyPtrMoveRobot) {
    CoolRobot coolRobot;
    poly_ptr<model::Robot> ptr{std::move(coolRobot)};
    MoveRobot(ptr);
    ASSERT_EQ(ptr.GetX(), 600);
    ASSERT_EQ(ptr.GetY(), 330);
}
