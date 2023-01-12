#include <cmath>
#include <string>

namespace test {

class TestRobot {
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
        return "TestRobot{x = " + std::to_string(x_) + ", y = " + std::to_string(y_) + "}";
    }

private:
    double angle_ = 0;
    double x_ = 0;
    double y_ = 0;
};

} // namespace test
