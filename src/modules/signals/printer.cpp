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
        AddMethodJsons(structJson, data.Signals, "signals", "grouped_signals");
        AddMethodJsons(structJson, data.Slots, "slots", "grouped_slots");
    }

    void AddMethodJsons(inja::json& structJson, std::vector<clang::CXXMethodDecl*> decls,
                        std::string_view name, std::string_view groupedName) {
        inja::json& arr = structJson[name];
        for (const auto* decl : decls) {
            arr.emplace_back(BuildMethodJson(*decl));
        }

        inja::json& grouped = structJson[groupedName];
        for (const auto& j : arr) {
            grouped[j["types"].get<std::string>()].emplace_back(j);
        }
    }

    inja::json BuildMethodJson(const clang::CXXMethodDecl& decl) {
        inja::json json;
        json["name"] = decl.getNameAsString();
        json["signature"] = StringUtil::GetSignature(decl);
        json["args"] = StringUtil::JoinArgs(decl);
        json["types"] = StringUtil::JoinTypes(decl);
        return json;
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
