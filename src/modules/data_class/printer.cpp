#include "printer.h"

#include <inja/inja.hpp>

#include <lib/file/file.h>
#include <lib/string_util/string_util.h>

namespace Waffle::DataClass {

namespace {

#include "template.cpp.data"
const std::string TEMPLATE = reinterpret_cast<char*>(template_cpp);

class Printer {
public:
    explicit Printer(Context& ctx, const ClassDatas& datas)
        : Ctx_{ctx}
        , Datas_{datas}
    {
    }

    void Print() {
        const std::string_view inFile = StringUtil::AfterLastSlash(Ctx_.InFile);
        const std::string changedInFile = StringUtil::InsertBeforeExt(inFile, "data_class");
        const std::string outputFile = std::string{StringUtil::RemoveLastExt(changedInFile)} + ".h";
        auto& printer = Ctx_.FileManager.GetOrCreateFilePrinter(outputFile);

        inja::json dataJson;
        dataJson["source_file"] = StringUtil::RemoveLastExt(inFile);

        inja::Environment env;
        env.set_trim_blocks(true);
        printer << env.render(TEMPLATE, dataJson);
    }

private:
    Context& Ctx_;
    const ClassDatas& Datas_;
};

} // namespace

void Print(Context& ctx, const ClassDatas& datas) {
    Printer printer{ctx, datas};
    printer.Print();
}

} // namespace Waffle::DataClass
