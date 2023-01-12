#include <gtest/gtest.h>
#include <waffle/modules/json_dump/json_dump.h>
#include "books_library/books_library.h"

using namespace Waffle;

namespace {

constexpr std::string_view JSON_DUMP = R"(
{
    "address": {
        "lat": 51.507351,
        "lon": -0.127696
    },
    "books": [
        {
            "author": "Oscar Wilde",
            "name": "The Picture of Dorian Gray",
            "year": 1890
        },
        {
            "author": "Ray Bradbury",
            "name": "Fahrenheit 451",
            "year": 1953
        },
        {
            "author": "Arkady and Boris Strugatsky",
            "name": "Roadside Picnic",
            "year": 1972
        }
    ],
    "description": null
})";

model::Library BuildBooksLibraryObject() {
    model::Library l;
    l.Address = model::LatLon{.Lat = 51.507351, .Lon = -0.127696};
    l.Books.push_back(model::Book{
        .Name = "The Picture of Dorian Gray",
        .Author = "Oscar Wilde",
        .Year = 1890,
    });
    l.Books.push_back(model::Book{
        .Name = "Fahrenheit 451",
        .Author = "Ray Bradbury",
        .Year = 1953,
    });
    l.Books.push_back(model::Book{
        .Name = "Roadside Picnic",
        .Author = "Arkady and Boris Strugatsky",
        .Year = 1972,
    });
    return l;
}

} // namespace

TEST(JsonDump, BooksLibraryToJson) {
    auto obj = BuildBooksLibraryObject();
    ASSERT_STREQ(("\n" + ToJson(obj).dump(/*indent=*/4)).data(), JSON_DUMP.data());
}

TEST(JsonDump, BooksLibraryFromJson) {
    auto json = nlohmann::json::parse(JSON_DUMP);
    auto obj = FromJson<model::Library>(json);
    ASSERT_STREQ(("\n" + ToJson(obj).dump(/*indent=*/4)).data(), JSON_DUMP.data());
}
