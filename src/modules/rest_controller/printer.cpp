#include "printer.h"

#include <clang/AST/ASTContext.h>

#include <inja/inja.hpp>

#include <lib/file/file.h>
#include <lib/string_util/string_util.h>

namespace Waffle::RestController {

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
        const std::string outputFile = StringUtil::InsertBeforeExt(inFile, "rest_controller");
        auto& printer = Ctx_.FileManager.GetOrCreateFilePrinter(outputFile);

        DataJson_["source_file"] = StringUtil::RemoveLastExt(inFile);
        for (const auto& data : Datas_) {
            AddRestController(data);
        }

        llvm::errs() << "GOT DATA " << DataJson_.dump(4) << "\n";

        inja::Environment env;
        env.set_trim_blocks(true);
        printer << env.render(TEMPLATE, DataJson_);
    }

private:
    void AddRestController(const StructData& data) {
        auto& structJson = DataJson_["structs"].emplace_back();
        structJson["name"] = StringUtil::QualifiedName(*data.Decl);

        for (const auto& methodData : data.MethodDatas) {
            const auto& decl = *methodData.Decl;
            auto& methodJson = structJson["methods"].emplace_back();
            methodJson["name"] = decl.getNameAsString();
            methodJson["return_type"] = decl.getReturnType().getAsString();
            methodJson["http_method"] = methodData.HttpMethod;
            methodJson["mapping"] = methodData.Mapping;

            auto& paramsJson = methodJson["params"];
            paramsJson = nlohmann::json::array();
            for (const auto& paramData : methodData.ParamDatas) {
                const auto& decl = *paramData.Decl;
                auto& paramJson = paramsJson.emplace_back();
                paramJson["name"] = decl.getNameAsString();
                paramJson["type"] = GetNameWithoutTagKeyword(decl);
                paramJson["command"] = paramData.Command;
            }
        }
    }

private:
    std::string GetNameWithoutTagKeyword(const clang::ValueDecl& decl) {
        static const auto printingPolicy = std::invoke([this]{
            auto p = Ctx_.AstContext.getPrintingPolicy();
            p.SuppressTagKeyword = true;
            return p;
        });
        return decl.getType().getAsString(printingPolicy);
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

} // namespace Waffle::RestController
