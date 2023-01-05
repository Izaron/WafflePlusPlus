#include <gtest/gtest.h>
#include <waffle/modules/enum_serializer/enum_serializer.h>
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
    ASSERT_EQ(FromString<T>("Foo"), T::Foo);
    AssertThrows<T>("Bar", R"(Can't parse value "Bar" to enum type "The::Longest::Namespace::Very::Long::Qualified::Name::Value")");

    ASSERT_EQ(ToString(T::Foo), "Foo");

    auto values = GetAllEnumValues<T>();
    ASSERT_EQ(values.size(), 1);
    ASSERT_EQ(values[0], T::Foo);
}
