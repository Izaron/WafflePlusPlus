#include <optional>
#include <string>
#include <vector>

namespace model {

struct book {
    std::string name;
    std::string author;
    int year;
};

struct latlon {
    double lat;
    double lon;
};

struct library {
    std::vector<book> books;
    std::optional<std::string> description;
    latlon address;
};

} // namespace model
