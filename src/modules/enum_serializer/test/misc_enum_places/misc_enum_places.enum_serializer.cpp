// Generated by the Waffle++ code generator. DO NOT EDIT!
// source: misc_enum_places.h

#include <vector>
#include <stdexcept>

#include <waffle/modules/enum_serializer/enum_serializer.h>

#include "misc_enum_places.h"

namespace Waffle {

template<>
Color FromString<Color>(std::string_view value) {
    if (value == "Red") {
        return Color::Red;
    }
    if (value == "Green") {
        return Color::Green;
    }
    if (value == "Blue") {
        return Color::Blue;
    }
    if (value == "Cyan") {
        return Color::Cyan;
    }
    if (value == "Magenta") {
        return Color::Magenta;
    }
    if (value == "Yellow") {
        return Color::Yellow;
    }
    if (value == "Black") {
        return Color::Black;
    }
    throw std::runtime_error("Can't parse value \"" + std::string{value} + "\" to enum type \"Color\"");
}

template<>
Color FromStringOrDefault<Color>(std::string_view value, Color defaultResult) {
    if (value == "Red") {
        return Color::Red;
    }
    if (value == "Green") {
        return Color::Green;
    }
    if (value == "Blue") {
        return Color::Blue;
    }
    if (value == "Cyan") {
        return Color::Cyan;
    }
    if (value == "Magenta") {
        return Color::Magenta;
    }
    if (value == "Yellow") {
        return Color::Yellow;
    }
    if (value == "Black") {
        return Color::Black;
    }
    return defaultResult;
}

template<>
std::string_view ToString(Color value) {
    switch (value) {
        case Color::Red: {
            return "Red";
        }
        case Color::Green: {
            return "Green";
        }
        case Color::Blue: {
            return "Blue";
        }
        case Color::Cyan: {
            return "Cyan";
        }
        case Color::Magenta: {
            return "Magenta";
        }
        case Color::Yellow: {
            return "Yellow";
        }
        case Color::Black: {
            return "Black";
        }
    }
    __builtin_unreachable();
}

template<>
std::span<const Color> GetAllEnumValues() {
    static const std::vector<Color> values{
        Color::Red,
        Color::Green,
        Color::Blue,
        Color::Cyan,
        Color::Magenta,
        Color::Yellow,
        Color::Black,
    };
    return values;
}

template<>
OsManager::Type FromString<OsManager::Type>(std::string_view value) {
    if (value == "LINUX") {
        return OsManager::LINUX;
    }
    if (value == "WINDOWS") {
        return OsManager::WINDOWS;
    }
    if (value == "OSX") {
        return OsManager::OSX;
    }
    throw std::runtime_error("Can't parse value \"" + std::string{value} + "\" to enum type \"OsManager::Type\"");
}

template<>
OsManager::Type FromStringOrDefault<OsManager::Type>(std::string_view value, OsManager::Type defaultResult) {
    if (value == "LINUX") {
        return OsManager::LINUX;
    }
    if (value == "WINDOWS") {
        return OsManager::WINDOWS;
    }
    if (value == "OSX") {
        return OsManager::OSX;
    }
    return defaultResult;
}

template<>
std::string_view ToString(OsManager::Type value) {
    switch (value) {
        case OsManager::LINUX: {
            return "LINUX";
        }
        case OsManager::WINDOWS: {
            return "WINDOWS";
        }
        case OsManager::OSX: {
            return "OSX";
        }
    }
    __builtin_unreachable();
}

template<>
std::span<const OsManager::Type> GetAllEnumValues() {
    static const std::vector<OsManager::Type> values{
        OsManager::LINUX,
        OsManager::WINDOWS,
        OsManager::OSX,
    };
    return values;
}

template<>
The::Longest::Namespace::Very::Long::Qualified::Name::Value FromString<The::Longest::Namespace::Very::Long::Qualified::Name::Value>(std::string_view value) {
    if (value == "Foo") {
        return The::Longest::Namespace::Very::Long::Qualified::Name::Value::Foo;
    }
    throw std::runtime_error("Can't parse value \"" + std::string{value} + "\" to enum type \"The::Longest::Namespace::Very::Long::Qualified::Name::Value\"");
}

template<>
The::Longest::Namespace::Very::Long::Qualified::Name::Value FromStringOrDefault<The::Longest::Namespace::Very::Long::Qualified::Name::Value>(std::string_view value, The::Longest::Namespace::Very::Long::Qualified::Name::Value defaultResult) {
    if (value == "Foo") {
        return The::Longest::Namespace::Very::Long::Qualified::Name::Value::Foo;
    }
    return defaultResult;
}

template<>
std::string_view ToString(The::Longest::Namespace::Very::Long::Qualified::Name::Value value) {
    switch (value) {
        case The::Longest::Namespace::Very::Long::Qualified::Name::Value::Foo: {
            return "Foo";
        }
    }
    __builtin_unreachable();
}

template<>
std::span<const The::Longest::Namespace::Very::Long::Qualified::Name::Value> GetAllEnumValues() {
    static const std::vector<The::Longest::Namespace::Very::Long::Qualified::Name::Value> values{
        The::Longest::Namespace::Very::Long::Qualified::Name::Value::Foo,
    };
    return values;
}

} // namespace Waffle
