#include "printer.h"

#include <clang/AST/ASTContext.h>

#include <inja/inja.hpp>

#include <lib/file/file.h>
#include <lib/string_util/string_util.h>

namespace Waffle::Signals {

namespace {

#include "template.cpp.data"
const std::string TEMPLATE{(char*)template_cpp, template_cpp_len};

class Printer {
public:
    explicit Printer(Context& ctx, const StructDatas& datas)
        : Ctx_{ctx}
        , Datas_{datas}
    {}

    void Print() {
        const std::string_view inFile = StringUtil::AfterLastSlash(Ctx_.InFile);
        const std::string outputFile = StringUtil::InsertBeforeExt(inFile, "signals");
        auto& printer = Ctx_.FileManager.GetOrCreateFilePrinter(outputFile);

        DataJson_["source_file"] = StringUtil::RemoveLastExt(inFile);

        for (const auto& data : Datas_) {
            AddStructData(data);
        }
        llvm::errs() << "GOT DATA " << DataJson_.dump(4) << "\n";

        inja::Environment env;
        env.set_trim_blocks(true);
        printer << env.render(TEMPLATE, DataJson_);
    }

private:
    void AddStructData(const StructData& data) {
        auto& structJson = DataJson_["structs"].emplace_back();
        structJson["qualified_name"] = StringUtil::QualifiedName(*data.Decl);
        for (const auto* signalDecl : data.Signals) {
            auto& signalJson = structJson["signals"].emplace_back();
            signalJson["name"] = signalDecl->getNameAsString();
            signalJson["signature"] = StringUtil::GetSignature(*signalDecl);
            signalJson["args"] = StringUtil::JoinArgs(*signalDecl);
        }
    }

private:
    Context& Ctx_;
    const StructDatas& Datas_;
    inja::json DataJson_;
};

} // namespace

void Print(Context& ctx, const StructDatas& datas) {
    Printer printer{ctx, datas};
    printer.Print();
}

} // namespace Waffle::Signals
