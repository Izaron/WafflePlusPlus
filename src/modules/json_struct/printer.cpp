#include "printer.h"

#include <lib/file/file.h>
#include <lib/string_util/string_util.h>

namespace Waffle::JsonStruct {

namespace {

#include "template.cpp.data"
const std::string TEMPLATE = reinterpret_cast<char*>(template_cpp);

class StructDataPrinter {
public:
    explicit StructDataPrinter(Context& ctx, const StructDatas& datas)
        : Ctx_{ctx}
        , Datas_{datas}
    {}

    void Print() {
        const std::string_view inFile = StringUtil::AfterLastSlash(Ctx_.InFile);
        const std::string outputFile = StringUtil::InsertBeforeExt(inFile, "json_struct");
        auto& printer = Ctx_.FileManager.GetOrCreateFilePrinter(outputFile);
    }

private:
    Context& Ctx_;
    const StructDatas& Datas_;
};

} // namespace

void PrintStructDatas(Context& ctx, const StructDatas& structDatas) {
    StructDataPrinter printer{ctx, structDatas};
    printer.Print();
}

} // namespace Waffle::JsonStruct
