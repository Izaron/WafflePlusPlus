#include <iostream>

#include <clang/Tooling/CompilationDatabase.h>

#include <lib/module/registry.h>
#include <lib/file/file_impl.h>

using namespace Waffle;

//int main(int argc, const char** argv) {
    //std::string errorMsg;
    //auto db = clang::tooling::CompilationDatabase::loadFromDirectory(".", errorMsg);
    //if (!db) {
        //llvm::errs() << "database not loaded, error:\n" << errorMsg;
        //return 1;
    //}

    //std::cout << "Hello world" << std::endl;
    //for (const auto& file : db->getAllFiles()) {
        //std::cout << file << std::endl;
    //}
//}

int main(int argc, const char** argv) {
    for (const auto& module : ModuleRegistry::GetModules()) {
        llvm::errs() << "module " << module->Name() << " is available\n";
    }

    if (argc < 2) {
        llvm::errs() << "please provide path!\n";
        return 1;
    }

    FileManager fileMgr{argv[1]};
    Context ctx{
        .FileMgr = fileMgr,
    };
    for (const auto& module : ModuleRegistry::GetModules()) {
        module->Do(ctx);
    }
}
