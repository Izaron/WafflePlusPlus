#include "printer.h"

#include <unordered_set>

#include <clang/AST/DeclTemplate.h>
#include <inja/inja.hpp>

#include <lib/file/file.h>
#include <lib/string_util/string_util.h>

namespace Waffle::PolyPtr {

namespace {

#include "template.cpp.data"
const std::string TEMPLATE{(char*)template_cpp, template_cpp_len};

class Printer {
public:
    explicit Printer(Context& ctx, const StructDecls& decls)
        : Ctx_{ctx}
        , Decls_{decls}
    {}

    void Print() {
        const std::string_view inFile = StringUtil::AfterLastSlash(Ctx_.InFile);
        const std::string changedInFile = StringUtil::InsertBeforeExt(inFile, "poly_ptr");
        const std::string outputFile = std::string{StringUtil::RemoveLastExt(changedInFile)} + ".h";
        auto& printer = Ctx_.FileManager.GetOrCreateFilePrinter(outputFile);

        DataJson_["source_file"] = StringUtil::RemoveLastExt(inFile);
        for (const auto* decl : Decls_) {
            AddDecl(decl);
        }

        llvm::errs() << "GOT DATA " << DataJson_.dump(4) << "\n";

        inja::Environment env;
        env.set_trim_blocks(true);
        printer << env.render(TEMPLATE, DataJson_);
    }

private:
    void AddDecl(const StructDecl decl) {
        auto& structJson = DataJson_["structs"].emplace_back();
        structJson["name"] = decl->getNameAsString();
        structJson["qualified_name"] = StringUtil::QualifiedName(*decl);

        for (const auto methodDecl : decl->methods()) {
            auto& methodJson = structJson["methods"].emplace_back();
            methodJson["name"] = methodDecl->getNameAsString();
            methodJson["return_type"] = methodDecl->getReturnType().getAsString();
            methodJson["signature"] = GetSignature(*methodDecl);
            methodJson["args"] = JoinArgs(*methodDecl);
            methodJson["qualifiers"] = GetQualifiers(*methodDecl);
        }
    }

    template<typename Func>
    std::string JoinParams(const clang::CXXMethodDecl& methodDecl, Func func) {
        std::stringstream ss;
        for (const auto param : methodDecl.parameters()) {
            if (ss.rdbuf()->in_avail()) { // aka `if (!ss.str())`
                ss << ", ";
            }
            ss << func(*param);
        }
        return ss.str();
    }

    std::string GetSignature(const clang::CXXMethodDecl& methodDecl) {
        return JoinParams(methodDecl, [](const auto& param) {
            return param.getType().getAsString() + " " + param.getNameAsString();
        });
    }

    std::string JoinArgs(const clang::CXXMethodDecl& methodDecl) {
        return JoinParams(methodDecl, [](const auto& param) {
            return param.getNameAsString();
        });
    }

    std::string GetQualifiers(const clang::CXXMethodDecl& methodDecl) {
        if (methodDecl.isConst()) {
            return " const";
        }
        return "";
    }

private:
    Context& Ctx_;
    const StructDecls& Decls_;
    inja::json DataJson_;
};

} // namespace

void Print(Context& ctx, const StructDecls& decls) {
    Printer printer{ctx, decls};
    printer.Print();
}

} // namespace Waffle::PolyPtr
