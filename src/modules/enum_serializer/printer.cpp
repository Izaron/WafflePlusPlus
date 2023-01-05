#include "printer.h"

#include <lib/file/nice_printer.h>
#include <lib/string_util/string_util.h>

namespace Waffle::EnumSerializer {

namespace {

constexpr std::string_view INSERT_BEFORE_EXT = "enum_serializer";

class EnumDataPrinter {
public:
    explicit EnumDataPrinter(Context& ctx, const EnumDatas& datas)
        : Ctx_{ctx}
        , Datas_{datas}
    {}

    void Print() {
        const std::string_view inFile = StringUtil::AfterLastSlash(Ctx_.InFile);
        const std::string outputFile = StringUtil::InsertBeforeExt(inFile, INSERT_BEFORE_EXT);
        auto printer = NicePrinter{Ctx_.FileManager.GetOrCreateFilePrinter(outputFile)};

        printer.AddPreabmle(StringUtil::RemoveLastExt(inFile));
        printer.Include("waffle/modules/enum_serializer/enum_serializer.h");
        printer.NewLine();
        printer.Include(StringUtil::RemoveLastExt(Ctx_.InFile));
        printer.NewLine();

        printer.OpenNamespace();

        for (const auto& data : Datas_) {
            const std::string enumType = StringUtil::QualifiedName(*data.Decl);
            printer << "template<>\n";
            printer << enumType << " FromString<" << enumType << ">(std::string_view value) {\n";
            printer.AddIndent();
            for (const auto& [constantDecl, stringValues] : data.Constants) {
                const std::string enumConstantName = StringUtil::QualifiedName(*constantDecl);
                for (const auto& stringValue : stringValues) {
                    printer << "if (value == \"" << stringValue << "\") {\n";
                    printer.AddIndent();
                    printer << "return " << enumConstantName << ";\n";
                    printer.DecreaseIndent();
                    printer << "}\n";
                }
            }
            printer << "// TODO: throw\n";
            printer.DecreaseIndent();
            printer << "}\n\n";
        }

        printer.CloseNamespace();
    }

private:
    Context& Ctx_;
    const EnumDatas& Datas_;
};

} // namespace

void PrintEnumDatas(Context& ctx, const EnumDatas& enumDatas) {
    EnumDataPrinter printer{ctx, enumDatas};
    printer.Print();
}

} // namespace Waffle::EnumSerializer
