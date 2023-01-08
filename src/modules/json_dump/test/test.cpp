#include <gtest/gtest.h>
#include <waffle/modules/json_dump/json_dump.h>
#include <nlohmann/json.hpp>
#include "books_library/books_library.h"

using namespace Waffle;

TEST(JsonDump, BooksLibrary) {
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

    const std::string dumpJsonStr = DumpJson(l);
    nlohmann::json dumpJson = nlohmann::json::parse(dumpJsonStr);
    std::string dumpJsonBeatufilStr = dumpJson.dump(/*indent=*/4);

    constexpr std::string_view expected = R"(
{
    "address": {
        "lat": 51.5074,
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
    ASSERT_STREQ(("\n" + dumpJsonBeatufilStr).data(), expected.data());
}
