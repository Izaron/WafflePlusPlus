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
    ASSERT_EQ(FromString<Color>("Red"), Color::Red);
    ASSERT_EQ(FromString<Color>("Green"), Color::Green);
    ASSERT_EQ(FromString<Color>("Blue"), Color::Blue);
    ASSERT_EQ(FromString<Color>("Cyan"), Color::Cyan);
    ASSERT_EQ(FromString<Color>("Magenta"), Color::Magenta);
    ASSERT_EQ(FromString<Color>("Yellow"), Color::Yellow);
    ASSERT_EQ(FromString<Color>("Black"), Color::Black);
    AssertThrows<Color>("Pink", R"(Can't parse value "Pink" to enum type "Color")");
}

TEST(EnumSerializer, MiscEnumPlacesOsManager) {
    ASSERT_EQ(FromString<OsManager::Type>("LINUX"), OsManager::LINUX);
    ASSERT_EQ(FromString<OsManager::Type>("WINDOWS"), OsManager::WINDOWS);
    ASSERT_EQ(FromString<OsManager::Type>("OSX"), OsManager::OSX);
    AssertThrows<OsManager::Type>("FREEBSD", R"(Can't parse value "FREEBSD" to enum type "OsManager::Type")");
}

TEST(EnumSerializer, MiscEnumPlacesVeryLongName) {
    using T = The::Longest::Namespace::Very::Long::Qualified::Name::Value;
    ASSERT_EQ(FromString<T>("Foo"), T::Foo);
    AssertThrows<T>("Bar", R"(Can't parse value "Bar" to enum type "The::Longest::Namespace::Very::Long::Qualified::Name::Value")");
}
