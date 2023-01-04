#include "file_impl.h"

namespace Waffle {

FilePrinter::FilePrinter(std::ofstream&& stream)
    : stream_{std::move(stream)}
{}

void FilePrinter::Print(std::string_view s) {
    stream_ << s;
}

FileManager::FileManager(std::string prefix)
    : prefix_{prefix}
{
    std::filesystem::create_directories(prefix_);
}

IFilePrinter& FileManager::GetOrCreateFilePrinter(std::string_view filename) {
    if (const auto iter = printers_.find(filename); iter != printers_.end()) {
        return iter->second;
    }
    const auto [iter, _] = printers_.emplace(filename, std::ofstream{prefix_ / filename});
    return iter->second;
}

std::vector<std::string> FileManager::GetGeneratedFilesNames() const {
    std::vector<std::string> names;
    for (const auto& iter : printers_) {
        names.emplace_back(iter.first);
    }
    return names;
}

} // namespace Waffle
