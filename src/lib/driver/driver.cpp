#include <clang/Frontend/FrontendActions.h>
#include <clang/Tooling/CompilationDatabase.h>
#include <clang/Tooling/Tooling.h>

#include <lib/module/registry.h>
#include <lib/file/file_impl.h>

using namespace Waffle;

class WaffleFrontendAction : public clang::ASTFrontendAction {
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
        clang::CompilerInstance& /*compiler*/, llvm::StringRef inFile) override
    {
        llvm::errs() << "look " << inFile << "\n";
        return nullptr;
    }
};

class WaffleFrontendActionFactory : public clang::tooling::FrontendActionFactory {
public:
    std::unique_ptr<clang::FrontendAction> create() override {
        return std::make_unique<WaffleFrontendAction>();
    }
};

int main(int argc, const char** argv) {
    for (const auto& module : ModuleRegistry::GetModules()) {
        llvm::errs() << "module \"" << module->Name() << "\" is available\n";
    }

    if (argc < 4) {
        llvm::errs() << "please provide paths:\n";
        llvm::errs() << "argv[1] = binary dir with compile_commands.json\n";
        llvm::errs() << "argv[2] = path to source .cpp file\n";
        llvm::errs() << "argv[3] = dir where write generated files\n";
        return 1;
    }

    const std::string binaryDir{argv[1]};
    const std::string filePath{argv[2]};
    const std::string outputDir{argv[3]};

    std::string errorMsg;
    auto db = clang::tooling::CompilationDatabase::autoDetectFromDirectory(binaryDir, errorMsg);
    if (!db) {
        llvm::errs() << "compile database not loaded, error:\n" << errorMsg;
        return 1;
    }
    llvm::errs() << "loaded compile database with " << db->getAllFiles().size() << " files\n";

    auto action = std::make_unique<WaffleFrontendActionFactory>();
    clang::tooling::ClangTool tool(*db.get(), {filePath});
    if (!tool.run(action.get())) {
        llvm::errs() << "some problems...\n";
    }

    FileManager fileMgr{outputDir};
    Context ctx{
        .FileMgr = fileMgr,
    };

    for (const auto& module : ModuleRegistry::GetModules()) {
        module->Do(ctx);
    }
}
