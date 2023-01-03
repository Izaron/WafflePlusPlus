#pragma once

#include <string_view>

namespace Waffle {

class IFilePrinter {
public:
    virtual ~IFilePrinter() = default;

private:
    virtual void Print(std::string_view s) = 0;

public:
    friend IFilePrinter& operator<<(IFilePrinter& printer, std::string_view s) {
        printer.Print(s);
        return printer;
    }
};

class IFileManager {
public:
    virtual ~IFileManager() = default;
    virtual IFilePrinter& GetOrCreateFilePrinter(std::string_view filename) = 0;
};

} // namespace Waffle
