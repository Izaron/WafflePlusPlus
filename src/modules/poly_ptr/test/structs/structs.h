#include <string>

namespace model {

// @polymorphic
struct Robot {
    void Forward(double distance);
    void Turn(double degrees);
    void GoTo(double x, double y);
    double GetX() const;
    double GetY() const;
};

// @polymorphic
struct Stringer {
    std::string String() const;
};

} // namespace model
