#pragma once

#include <string_view>

namespace waffle {

class IFilePrinter {
private:
    virtual void Print(std::string_view s) = 0;

public:
    friend IFilePrinter& operator<<(std::string_view s, IFilePrinter& printer) {
        printer.Print(s);
        return printer;
    }
};

} // namespace waffle
