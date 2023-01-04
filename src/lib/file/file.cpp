#include "file.h"

#include <filesystem>
#include <fstream>
#include <map>

namespace Waffle {

namespace {

class FilePrinter : public IFilePrinter {
public:
    explicit FilePrinter(std::ofstream&& stream)
        : stream_{std::move(stream)}
    {}

private:
    void Print(std::string_view s) override {
        stream_ << s;
    }

private:
    std::ofstream stream_;
};

class FileManager : public IFileManager {
public:
    explicit FileManager(std::string prefix)
        : prefix_{prefix}
    {
        std::filesystem::create_directories(prefix_);
    }

    IFilePrinter& GetOrCreateFilePrinter(std::string_view filename) override {
        if (const auto iter = printers_.find(filename); iter != printers_.end()) {
            return iter->second;
        }
        const auto [iter, _] = printers_.emplace(filename, std::ofstream{prefix_ / filename});
        return iter->second;
    }

    std::vector<std::string> GetGeneratedFilesNames() const override {
        std::vector<std::string> names;
        for (const auto& iter : printers_) {
            names.emplace_back(iter.first);
        }
        return names;
    }

private:
    std::filesystem::path prefix_;
    std::map<std::string, FilePrinter, std::less<>> printers_;
};

} // namespace

std::unique_ptr<IFileManager> BuildFileManager(std::string prefix) {
    return std::make_unique<FileManager>(prefix);
}

} // namespace Waffle
