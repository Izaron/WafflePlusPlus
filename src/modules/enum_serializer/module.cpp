#include <clang/Tooling/CompilationDatabase.h>

#include <iostream>

int main(int argc, const char** argv) {
    std::string errorMsg;
    auto db = clang::tooling::CompilationDatabase::loadFromDirectory(".", errorMsg);
    if (!db) {
        llvm::errs() << "database not loaded, error:\n" << errorMsg;
        return 1;
    }

    std::cout << "Hello world" << std::endl;
    for (const auto& file : db->getAllFiles()) {
        std::cout << file << std::endl;
    }
}
