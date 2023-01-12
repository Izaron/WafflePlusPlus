#include <optional>
#include <string>
#include <vector>

namespace model {

struct Book {
    std::string Name; // @stringvalue name
    std::string Author; // @stringvalue author
    int Year; // @stringvalue year
};

struct LatLon {
    double Lat; // @stringvalue lat
    double Lon; // @stringvalue lon
};

// @jsonable
struct Library {
    std::vector<Book> Books; // @stringvalue books
    std::optional<std::string> Description; // @stringvalue description
    LatLon Address; // @stringvalue address
};

} // namespace model
