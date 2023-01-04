#pragma once

#include <memory>
#include <string_view>
#include <vector>

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
    virtual std::vector<std::string> GetGeneratedFilesNames() const = 0;
};

std::unique_ptr<IFileManager> BuildFileManager(std::string prefix);

} // namespace Waffle
