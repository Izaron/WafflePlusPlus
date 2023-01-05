#include <gtest/gtest.h>
#include <waffle/modules/enum_serializer/enum_serializer.h>
#include "custom_names/custom_names.h"
#include "misc_enum_places/misc_enum_places.h"

using namespace Waffle;

template<typename EnumType>
static void AssertThrows(std::string_view value, std::string_view message) {
    EXPECT_THROW(FromString<EnumType>(value), std::runtime_error);
    try {
        FromString<EnumType>(value);
    } catch (const std::runtime_error& e) {
        ASSERT_STREQ(e.what(), message.data());
    }
}

TEST(EnumSerializer, MiscEnumPlacesColor) {
    // check FromString
    ASSERT_EQ(FromString<Color>("Red"), Color::Red);
    ASSERT_EQ(FromString<Color>("Green"), Color::Green);
    ASSERT_EQ(FromString<Color>("Blue"), Color::Blue);
    ASSERT_EQ(FromString<Color>("Cyan"), Color::Cyan);
    ASSERT_EQ(FromString<Color>("Magenta"), Color::Magenta);
    ASSERT_EQ(FromString<Color>("Yellow"), Color::Yellow);
    ASSERT_EQ(FromString<Color>("Black"), Color::Black);
    AssertThrows<Color>("Pink", R"(Can't parse value "Pink" to enum type "Color")");

    // check FromStringOrDefault
    ASSERT_EQ(FromStringOrDefault<Color>("Red", Color::Cyan), Color::Red);
    ASSERT_EQ(FromStringOrDefault<Color>("Pink", Color::Cyan), Color::Cyan);

    // check ToString
    ASSERT_EQ(ToString(Color::Red), "Red");
    ASSERT_EQ(ToString(Color::Green), "Green");
    ASSERT_EQ(ToString(Color::Blue), "Blue");
    ASSERT_EQ(ToString(Color::Cyan), "Cyan");
    ASSERT_EQ(ToString(Color::Magenta), "Magenta");
    ASSERT_EQ(ToString(Color::Yellow), "Yellow");
    ASSERT_EQ(ToString(Color::Black), "Black");

    // check GetAllEnumValues
    const std::vector<Color> expectedValues{
        Color::Red, Color::Green, Color::Blue,
        Color::Cyan, Color::Magenta, Color::Yellow,
        Color::Black,
    };
    auto realValues = GetAllEnumValues<Color>();
    ASSERT_EQ(realValues.size(), expectedValues.size());
    for (size_t i = 0; i < realValues.size(); ++i) {
        ASSERT_EQ(realValues[i], expectedValues[i]);
    }
}

TEST(EnumSerializer, MiscEnumPlacesOsManager) {
    // check FromString
    ASSERT_EQ(FromString<OsManager::Type>("LINUX"), OsManager::LINUX);
    ASSERT_EQ(FromString<OsManager::Type>("WINDOWS"), OsManager::WINDOWS);
    ASSERT_EQ(FromString<OsManager::Type>("OSX"), OsManager::OSX);
    AssertThrows<OsManager::Type>("FREEBSD", R"(Can't parse value "FREEBSD" to enum type "OsManager::Type")");

    // check FromStringOrDefault
    ASSERT_EQ(FromStringOrDefault<OsManager::Type>("LINUX", OsManager::OSX), OsManager::LINUX);
    ASSERT_EQ(FromStringOrDefault<OsManager::Type>("FREEBSD", OsManager::OSX), OsManager::OSX);

    // check ToString
    ASSERT_EQ(ToString(OsManager::LINUX), "LINUX");
    ASSERT_EQ(ToString(OsManager::WINDOWS), "WINDOWS");
    ASSERT_EQ(ToString(OsManager::OSX), "OSX");

    // check GetAllEnumValues
    const std::vector<OsManager::Type> expectedValues{
        OsManager::LINUX, OsManager::WINDOWS, OsManager::OSX,
    };
    auto realValues = GetAllEnumValues<OsManager::Type>();
    ASSERT_EQ(realValues.size(), expectedValues.size());
    for (size_t i = 0; i < realValues.size(); ++i) {
        ASSERT_EQ(realValues[i], expectedValues[i]);
    }
}

TEST(EnumSerializer, MiscEnumPlacesVeryLongName) {
    using T = The::Longest::Namespace::Very::Long::Qualified::Name::Value;

    // check FromString
    ASSERT_EQ(FromString<T>("Foo"), T::Foo);
    AssertThrows<T>("Bar", R"(Can't parse value "Bar" to enum type "The::Longest::Namespace::Very::Long::Qualified::Name::Value")");

    // check FromStringOrDefault
    ASSERT_EQ(FromStringOrDefault<T>("Foo", T::Foo), T::Foo);
    ASSERT_EQ(FromStringOrDefault<T>("Bar", T::Foo), T::Foo);

    // check ToString
    ASSERT_EQ(ToString(T::Foo), "Foo");

    // check GetAllEnumValues
    auto values = GetAllEnumValues<T>();
    ASSERT_EQ(values.size(), 1);
    ASSERT_EQ(values[0], T::Foo);
}

TEST(EnumSerializer, CustomNamesBookColor) {
    // check FromString
    ASSERT_EQ(FromString<BookColor>("red"), BookColor::kRed);
    ASSERT_EQ(FromString<BookColor>("rot"), BookColor::kRed);
    ASSERT_EQ(FromString<BookColor>("rouge"), BookColor::kRed);
    ASSERT_EQ(FromString<BookColor>("green"), BookColor::kGreen);
    ASSERT_EQ(FromString<BookColor>("grün"), BookColor::kGreen);
    ASSERT_EQ(FromString<BookColor>("vert"), BookColor::kGreen);
    ASSERT_EQ(FromString<BookColor>("blue"), BookColor::kBlue);
    ASSERT_EQ(FromString<BookColor>("blau"), BookColor::kBlue);
    ASSERT_EQ(FromString<BookColor>("bleu"), BookColor::kBlue);
    AssertThrows<BookColor>("pink", R"(Can't parse value "pink" to enum type "BookColor")");

    // check ToString
    ASSERT_EQ(ToString(BookColor::kRed), "red");
    ASSERT_EQ(ToString(BookColor::kGreen), "green");
    ASSERT_EQ(ToString(BookColor::kBlue), "blue");

    // check GetAllEnumValues
    ASSERT_EQ(GetAllEnumValues<BookColor>().size(), 3);
}

TEST(EnumSerializer, CustomNamesNumber) {
    // check FromString
    ASSERT_EQ(FromString<Number>("zero"), ZERO);
    ASSERT_EQ(FromString<Number>("null"), ZERO);
    ASSERT_EQ(FromString<Number>("none"), ZERO);
    ASSERT_EQ(FromString<Number>("empty"), ZERO);
    ASSERT_EQ(FromString<Number>("absent"), ZERO);
    ASSERT_EQ(FromString<Number>("missing"), ZERO);
    ASSERT_EQ(FromString<Number>("invalid"), ZERO);
    ASSERT_EQ(FromString<Number>("one"), ONE);
    ASSERT_EQ(FromString<Number>("single"), ONE);
    ASSERT_EQ(FromString<Number>("TWO"), TWO);
    ASSERT_EQ(FromString<Number>("THREE"), THREE);
    AssertThrows<Number>("four", R"(Can't parse value "four" to enum type "Number")");

    // check ToString
    ASSERT_EQ(ToString(ZERO), "zero");
    ASSERT_EQ(ToString(ONE), "one");
    ASSERT_EQ(ToString(TWO), "TWO");
    ASSERT_EQ(ToString(THREE), "THREE");

    // check GetAllEnumValues
    ASSERT_EQ(GetAllEnumValues<Number>().size(), 4);
}
