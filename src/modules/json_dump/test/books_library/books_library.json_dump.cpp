// Generated by the Waffle++ code generator. DO NOT EDIT!
// source: books_library.h

#include <array>
#include <deque>
#include <forward_list>
#include <list>
#include <optional>
#include <sstream>
#include <vector>

#include <waffle/modules/json_dump/json_dump.h>

#include "books_library.h"

namespace Waffle {

namespace Impl {

template<class, template<class...> class> inline constexpr bool is_specialization = false;
template<template<class...> class T, class... Args> inline constexpr bool is_specialization<T<Args...>, T> = true;

template<typename T> concept JsonNumber = std::is_arithmetic_v<T>;
template<typename T> concept JsonString = std::same_as<std::string, T>;
template<typename T> concept JsonBoolean = std::same_as<bool, T>;
template<typename T> concept JsonArray =
    is_specialization<T, std::vector> || is_specialization<T, std::deque> ||
    is_specialization<T, std::list> || is_specialization<T, std::forward_list> ||
    is_specialization<T, std::array>;
template<typename T> concept JsonObject = std::is_class_v<T>;
template<typename T> concept JsonNullable = is_specialization<T, std::optional>;

template<typename T>
void DumpRoot(std::stringstream& ss, const T& value);

template<JsonNumber T>
void DumpNumber(std::stringstream& ss, const T value) {
    ss << value;
}

template<JsonString T>
void DumpString(std::stringstream& ss, const T& value) {
    ss << '"' << value << '"';
}

template<JsonBoolean T>
void DumpBoolean(std::stringstream& ss, const T value) {
    if (value) {
        ss << "true";
    } else {
        ss << "false";
    }
}

template<JsonArray T>
void DumpArray(std::stringstream& ss, const T& value) {
    ss << "[";
    for (auto iter = value.begin(); iter != value.end(); ++iter) {
        if (iter != value.begin()) {
            ss << ",";
        }
        DumpRoot(ss, *iter);
    }
    ss << "]";
}

template<JsonNullable T>
inline void DumpNullable(std::stringstream& ss, const T& value) {
    if (value.has_value()) {
        DumpRoot(ss, *value);
    } else {
        ss << "null";
    }
}

template<JsonObject T>
void DumpObject(std::stringstream& ss, const T& value);

template<>
void DumpObject(std::stringstream& ss, const model::latlon& value) {
    ss << '{';
    ss << "\"lat\":";
    DumpNumber(ss, value.lat);
    ss << ',';
    ss << "\"lon\":";
    DumpNumber(ss, value.lon);
    ss << '}';
}

template<>
void DumpObject(std::stringstream& ss, const model::book& value) {
    ss << '{';
    ss << "\"name\":";
    DumpString(ss, value.name);
    ss << ',';
    ss << "\"author\":";
    DumpString(ss, value.author);
    ss << ',';
    ss << "\"year\":";
    DumpNumber(ss, value.year);
    ss << '}';
}

template<>
void DumpObject(std::stringstream& ss, const model::library& value) {
    ss << '{';
    ss << "\"books\":";
    DumpArray(ss, value.books);
    ss << ',';
    ss << "\"description\":";
    DumpNullable(ss, value.description);
    ss << ',';
    ss << "\"address\":";
    DumpObject(ss, value.address);
    ss << '}';
}

template<typename T>
void DumpRoot(std::stringstream& ss, const T& value) {
    if constexpr (JsonNullable<T>) {
        DumpNullable(ss, value);
    } else if constexpr (JsonNumber<T>) {
        DumpNumber(ss, value);
    } else if constexpr (JsonString<T>) {
        DumpString(ss, value);
    } else if constexpr (JsonBoolean<T>) {
        DumpBoolean(ss, value);
    } else if constexpr (JsonArray<T>) {
        DumpArray(ss, value);
    } else if constexpr (JsonObject<T>) {
        DumpObject(ss, value);
    }
}

} // namespace Impl

template<typename T>
std::string DumpJson(const T& value) {
    std::stringstream ss;
    Impl::DumpRoot(ss, value);
    return ss.str();
}

template std::string DumpJson<model::latlon>(const model::latlon&);
template std::string DumpJson<model::book>(const model::book&);
template std::string DumpJson<model::library>(const model::library&);

} // namespace Waffle