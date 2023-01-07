#include <clang/Frontend/FrontendActions.h>
#include <clang/Tooling/ArgumentsAdjusters.h>
#include <clang/Tooling/CompilationDatabase.h>
#include <clang/Tooling/Tooling.h>

#include <lib/file/file.h>
#include <lib/registry/registry.h>

#include <optional>
#include <sstream>

using namespace Waffle;

namespace {

class WaffleASTConsumer : public clang::ASTConsumer {
public:
    WaffleASTConsumer(IFileManager& fileManager, std::string inFile)
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
            module.Func(ctx);
        }
    }

private:
    IFileManager& FileManager_;
    std::string InFile_;
};

class WaffleFrontendAction : public clang::ASTFrontendAction {
public:
    WaffleFrontendAction(IFileManager& fileManager)
        : FileManager_{fileManager}
    {}

    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
        clang::CompilerInstance& /*compiler*/, llvm::StringRef inFile) override
    {
        return std::make_unique<WaffleASTConsumer>(FileManager_, std::string{inFile});
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

std::optional<std::string> BuildCommandsArgs() {
    std::vector<std::string_view> allCommands;
    for (const auto& module : ModuleRegistry::GetModules()) {
        allCommands.insert(allCommands.end(), module.Commands.begin(), module.Commands.end());
    }
    if (allCommands.empty()) {
        return std::nullopt;
    }
    std::stringstream ss;
    ss << "-fcomment-block-commands=";
    for (const auto command : allCommands) {
        ss << command << ",";
    }
    return ss.str();
}

clang::tooling::CommandLineArguments WaffleArgumentsAdjuster(
    const clang::tooling::CommandLineArguments& args, llvm::StringRef /*filename*/)
{
    auto result = args;
    result.emplace_back("-fparse-all-comments");
    if (auto arg = BuildCommandsArgs()) {
        llvm::errs() << "add custom commands: " << *arg << "\n";
        result.emplace_back(std::move(*arg));
    }
    return result;
}

} // namespace

int main(int argc, const char** argv) {
    for (const auto& module : ModuleRegistry::GetModules()) {
        llvm::errs() << "module \"" << module.Name << "\" is available\n";
    }

    if (argc < 4) {
        llvm::errs() << "please provide paths:\n";
        llvm::errs() << "argv[1] = current binary dir (with compile_commands.json)\n";
        llvm::errs() << "argv[2] = path to source .cpp file (real file path is argv[1]+argv[2])\n";
        llvm::errs() << "argv[3] = dir where to save generated files\n";
        return 1;
    }

    std::string binaryDir{argv[1]};
    std::string filePath{argv[2]};
    std::string outputDir{argv[3]};

    if (const auto pos = filePath.find_last_of('/'); pos != std::string::npos) {
        outputDir += "/";
        outputDir += filePath.substr(0, pos);
    }
    filePath = binaryDir + "/" + filePath;

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

    for (const auto& name : fileManager->GetGeneratedFilesNames()) {
        llvm::errs() << "added \"" << name << "\"\n";
    }

    return 0;
}
