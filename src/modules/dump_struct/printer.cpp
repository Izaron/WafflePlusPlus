#include "printer.h"

#include <lib/file/nice_printer.h>
#include <lib/string_util/string_util.h>

namespace Waffle::DumpStruct {

namespace {

constexpr std::string_view INSERT_BEFORE_EXT = "dump_struct";

class StructDataPrinter {
public:
    explicit StructDataPrinter(Context& ctx, const StructDatas& datas)
        : Ctx_{ctx}
        , Datas_{datas}
    {}

    void Print() {
        const std::string_view inFile = StringUtil::AfterLastSlash(Ctx_.InFile);
        const std::string outputFile = StringUtil::InsertBeforeExt(inFile, INSERT_BEFORE_EXT);
        Printer_ = NicePrinter{Ctx_.FileManager.GetOrCreateFilePrinter(outputFile)};

        Printer_.AddPreabmle(StringUtil::RemoveLastExt(inFile));
    }

private:
    Context& Ctx_;
    const StructDatas& Datas_;
    NicePrinter Printer_;
};

} // namespace

void PrintStructDatas(Context& ctx, const StructDatas& structDatas) {
    StructDataPrinter printer{ctx, structDatas};
    printer.Print();
}

} // namespace Waffle::DumpStruct
