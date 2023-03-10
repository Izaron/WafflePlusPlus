#include "printer.h"

#include <inja/inja.hpp>

#include <lib/file/file.h>
#include <lib/string_util/string_util.h>

namespace Waffle::EnumSerializer {

namespace {

#include "template.cpp.data"
const std::string TEMPLATE{(char*)template_cpp, template_cpp_len};

class Printer {
public:
    explicit Printer(Context& ctx, const EnumDatas& datas)
        : Ctx_{ctx}
        , Datas_{datas}
    {}

    void Print() {
        const std::string_view inFile = StringUtil::AfterLastSlash(Ctx_.InFile);
        const std::string outputFile = StringUtil::InsertBeforeExt(inFile, "enum_serializer");
        auto& printer = Ctx_.FileManager.GetOrCreateFilePrinter(outputFile);

        inja::json dataJson;
        dataJson["source_file"] = StringUtil::RemoveLastExt(inFile);
        for (const auto& data : Datas_) {
            auto& enumJson = dataJson["enums"].emplace_back();
            enumJson["name"] = StringUtil::QualifiedName(*data.Decl);

            for (const auto& [constantDecl, stringValues] : data.Constants) {
                auto& valueJson = enumJson["values"].emplace_back();
                valueJson["name"] = StringUtil::QualifiedName(*constantDecl);
                for (const auto& stringValue : stringValues) {
                    valueJson["strings"].emplace_back(stringValue);
                }
            }
        }
        printer << inja::render(TEMPLATE, dataJson);
    }

private:
    Context& Ctx_;
    const EnumDatas& Datas_;
};

} // namespace

void Print(Context& ctx, const EnumDatas& enumDatas) {
    Printer printer{ctx, enumDatas};
    printer.Print();
}

} // namespace Waffle::EnumSerializer
