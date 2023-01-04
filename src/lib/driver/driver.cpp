#include <clang/Frontend/FrontendActions.h>
#include <clang/Tooling/ArgumentsAdjusters.h>
#include <clang/Tooling/CompilationDatabase.h>
#include <clang/Tooling/Tooling.h>

#include <lib/file/file.h>
#include <lib/module/registry.h>

using namespace Waffle;

namespace {

class WaffleASTConsumer : public clang::ASTConsumer {
public:
    WaffleASTConsumer(IFileManager& fileManager, std::string_view inFile)
        : FileManager_{fileManager}
        , InFile_{inFile}
    {}

    void HandleTranslationUnit(clang::ASTContext &astContext) override {
        Context ctx{
            .FileManager = FileManager_,
            .InFile = InFile_,
            .AstContext = astContext,
        };
        for (const auto& module : ModuleRegistry::GetModules()) {
            module->Do(ctx);
        }
    }

private:
    IFileManager& FileManager_;
    std::string_view InFile_;
};

class WaffleFrontendAction : public clang::ASTFrontendAction {
public:
    WaffleFrontendAction(IFileManager& fileManager)
        : FileManager_{fileManager}
    {}

    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
        clang::CompilerInstance& /*compiler*/, llvm::StringRef inFile) override
    {
        return std::make_unique<WaffleASTConsumer>(FileManager_, inFile);
    }

private:
    IFileManager& FileManager_;
};

class WaffleFrontendActionFactory : public clang::tooling::FrontendActionFactory {
public:
    WaffleFrontendActionFactory(IFileManager& fileManager)
        : FileManager_{fileManager}
    {}

    std::unique_ptr<clang::FrontendAction> create() override {
        return std::make_unique<WaffleFrontendAction>(FileManager_);
    }

private:
    IFileManager& FileManager_;
};

clang::tooling::CommandLineArguments WaffleArgumentsAdjuster(
    const clang::tooling::CommandLineArguments& args, llvm::StringRef /*filename*/)
{
    auto result = args;
    result.emplace_back("-fparse-all-comments");
    result.emplace_back("-fcomment-block-commands=serializable");
    return result;
}

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

    auto fileManager = BuildFileManager(outputDir);
    auto action = std::make_unique<WaffleFrontendActionFactory>(*fileManager);
    clang::tooling::ClangTool tool(*db.get(), {filePath});
    tool.appendArgumentsAdjuster(WaffleArgumentsAdjuster);
    if (tool.run(action.get())) {
        llvm::errs() << "some problems with file \"" << filePath << "\"\n";
        return 1;
    }

    llvm::errs() << "generated files in directory \"" << outputDir << "\":\n";
    for (const auto& name : fileManager->GetGeneratedFilesNames()) {
        llvm::errs() << "added \"" << name << "\"\n";
    }

    return 0;
}
