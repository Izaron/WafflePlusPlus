#include <clang/Frontend/FrontendActions.h>
#include <clang/Tooling/CompilationDatabase.h>
#include <clang/Tooling/Tooling.h>

#include <lib/module/registry.h>
#include <lib/file/file_impl.h>

using namespace Waffle;

namespace {

class WaffleASTConsumer : public clang::ASTConsumer {
public:
    WaffleASTConsumer(IFileManager& fileMgr, std::string_view inFile)
        : FileMgr_{fileMgr}
        , InFile_{inFile}
    {}

    void HandleTranslationUnit(clang::ASTContext &astContext) override {
        Context ctx{
            .FileMgr = FileMgr_,
            .InFile = InFile_,
            .AstContext = astContext,
        };
        for (const auto& module : ModuleRegistry::GetModules()) {
            module->Do(ctx);
        }
    }

private:
    IFileManager& FileMgr_;
    std::string_view InFile_;
};

class WaffleFrontendAction : public clang::ASTFrontendAction {
public:
    WaffleFrontendAction(IFileManager& fileMgr)
        : FileMgr_{fileMgr}
    {}

    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
        clang::CompilerInstance& /*compiler*/, llvm::StringRef inFile) override
    {
        return std::make_unique<WaffleASTConsumer>(FileMgr_, inFile);
    }

private:
    IFileManager& FileMgr_;
};

class WaffleFrontendActionFactory : public clang::tooling::FrontendActionFactory {
public:
    WaffleFrontendActionFactory(IFileManager& fileMgr)
        : FileMgr_{fileMgr}
    {}

    std::unique_ptr<clang::FrontendAction> create() override {
        return std::make_unique<WaffleFrontendAction>(FileMgr_);
    }

private:
    IFileManager& FileMgr_;
};

} // namespace

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

    FileManager fileMgr{outputDir};
    auto action = std::make_unique<WaffleFrontendActionFactory>(fileMgr);
    clang::tooling::ClangTool tool(*db.get(), {filePath});
    if (tool.run(action.get())) {
        llvm::errs() << "some problems with file \"" << filePath << "\"\n";
    }

    return 0;
}
