#include <string>
#include <optional>

namespace model {

// @dataclass LatLon
struct LatLonStub {
    double latitude;
    double longitude;
};

// @dataclass Mountain
struct MountainStub {
    std::optional<std::string> name;
    std::string country;
    LatLonStub position;
    double peak;
};

} // namespace model
