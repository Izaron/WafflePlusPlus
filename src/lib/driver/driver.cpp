#include <iostream>

#include <clang/Tooling/CompilationDatabase.h>

#include <lib/module/registry.h>
#include <lib/file/file_impl.h>
#include <modules/enum_serializer/module.h>

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
    FileManager fileMgr{/*prefix=*/"."};
    Context ctx{
        .FileMgr = fileMgr,
    };
    llvm::errs() << "list modules:\n";
    for (const auto& module : ModuleRegistry::GetModules()) {
        llvm::errs() << "module " << module->Name() << " linked\n";
        module->Do(ctx);
    }
}
