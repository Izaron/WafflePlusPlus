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
            methodJson["signature"] = StringUtil::GetSignature(*methodDecl);
            methodJson["args"] = StringUtil::JoinArgs(*methodDecl);
            methodJson["qualifiers"] = GetQualifiers(*methodDecl);
        }
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
