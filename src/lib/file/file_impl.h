#pragma once

#include <filesystem>
#include <fstream>
#include <map>

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
    std::filesystem::path prefix_;
    std::map<std::string, FilePrinter, std::less<>> printers_;
};

} // namespace waffle
