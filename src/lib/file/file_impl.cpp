#include "file_impl.h"

namespace waffle {

FilePrinter::FilePrinter(std::ofstream&& stream)
    : stream_{std::move(stream)}
{}

void FilePrinter::Print(std::string_view s) {
    stream_ << s;
}

FileManager::FileManager(std::string prefix)
    : prefix_{prefix}
{}

IFilePrinter& FileManager::GetOrCreateFilePrinter(std::string_view filename) {
    const std::string fullPath = prefix_ + "/" + std::string{filename};
    if (const auto iter = printers_.find(fullPath); iter != printers_.end()) {
        return iter->second;
    }
    const auto [iter, _] = printers_.emplace(fullPath, std::ofstream{fullPath});
    return iter->second;
}

} // namespace waffle
