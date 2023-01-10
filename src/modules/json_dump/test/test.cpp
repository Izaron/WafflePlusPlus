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

model::library BuildBooksLibraryObject() {
    model::library l;
    l.address = model::latlon{.lat = 51.507351, .lon = -0.127696};
    l.books.push_back(model::book{
        .name = "The Picture of Dorian Gray",
        .author = "Oscar Wilde",
        .year = 1890,
    });
    l.books.push_back(model::book{
        .name = "Fahrenheit 451",
        .author = "Ray Bradbury",
        .year = 1953,
    });
    l.books.push_back(model::book{
        .name = "Roadside Picnic",
        .author = "Arkady and Boris Strugatsky",
        .year = 1972,
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
    auto obj = FromJson<model::library>(json);
    ASSERT_STREQ(("\n" + ToJson(obj).dump(/*indent=*/4)).data(), JSON_DUMP.data());
}
