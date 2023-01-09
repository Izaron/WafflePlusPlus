#include <string>
#include <optional>

namespace model {

struct LatLonStub {
};

// @dataclass Mountain
struct MountainStub {
    std::optional<std::string> name;
    std::string country;
    LatLonStub position;
};

} // namespace model
