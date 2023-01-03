#pragma once

#include <fstream>
#include <unordered_map>

#include "i_file.h"

namespace waffle {

class FilePrinter : public IFilePrinter {
public:
    explicit FilePrinter(std::ofstream&& stream);

private:
    void Print(std::string_view s) override;

private:
    std::ofstream stream_;
};

class FileManager : public IFileManager {
public:
    explicit FileManager(std::string prefix);
    IFilePrinter& GetOrCreateFilePrinter(std::string_view filename) override;

private:
    std::string prefix_;
    std::unordered_map<std::string, FilePrinter> printers_;
};

} // namespace waffle
