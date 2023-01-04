#include <iostream>

#include <clang/Tooling/CompilationDatabase.h>

#include <lib/module/registry.h>
#include <lib/file/file_impl.h>

using namespace Waffle;

int main(int argc, const char** argv) {
    for (const auto& module : ModuleRegistry::GetModules()) {
        llvm::errs() << "module " << module->Name() << " is available\n";
    }

    if (argc < 3) {
        llvm::errs() << "please provide path to source file and output directory\n";
        return 1;
    }

    std::string errorMsg;
    auto db = clang::tooling::CompilationDatabase::autoDetectFromSource(argv[1], errorMsg);
    if (!db) {
        llvm::errs() << "database not loaded, error:\n" << errorMsg;
        return 1;
    }

    for (const auto& file : db->getAllFiles()) {
        std::cout << file << std::endl;
    }

    FileManager fileMgr{argv[2]};
    Context ctx{
        .FileMgr = fileMgr,
    };
    for (const auto& module : ModuleRegistry::GetModules()) {
        module->Do(ctx);
    }
}
