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
        Printer_ = NicePrinter{Ctx_.FileManager.GetOrCreateFilePrinter(outputFile)};

        Printer_.AddPreabmle(StringUtil::RemoveLastExt(inFile));
        Printer_.Include("stdexcept");
        Printer_.Include("waffle/modules/enum_serializer/enum_serializer.h");
        Printer_.Include(StringUtil::RemoveLastExt(Ctx_.InFile));
        Printer_.NewLine();

        Printer_.OpenNamespace();
        for (const auto& data : Datas_) {
            PrintEnumData(data);
        }
        Printer_.CloseNamespace();
    }

private:
    void PrintEnumData(const EnumData& data) {
        PrintFromString(data);
        PrintToString(data);
    }

    void PrintFromString(const EnumData& data) {
        const std::string enumType = StringUtil::QualifiedName(*data.Decl);
        Printer_ << "template<>\n";
        Printer_ << enumType << " FromString<" << enumType << ">(std::string_view value) {\n";
        Printer_.AddIndent();
        for (const auto& [constantDecl, stringValues] : data.Constants) {
            const std::string enumConstantName = StringUtil::QualifiedName(*constantDecl);
            for (const auto& stringValue : stringValues) {
                Printer_ << "if (value == \"" << stringValue << "\") {\n";
                Printer_.AddIndent();
                Printer_ << "return " << enumConstantName << ";\n";
                Printer_.DecreaseIndent();
                Printer_ << "}\n";
            }
        }
        Printer_.Throw(R"("Can't parse value \"" + std::string{value} + "\" to enum type \")" + enumType + R"(\"")");
        Printer_.DecreaseIndent();
        Printer_ << "}\n\n";
    }

    void PrintToString(const EnumData& data) {
        const std::string enumType = StringUtil::QualifiedName(*data.Decl);
        Printer_ << "template<>\n";
        Printer_ << "std::string_view ToString(" << enumType << " value) {\n";
        Printer_.AddIndent();

        Printer_ << "switch (value) {\n";
        Printer_.AddIndent();
        for (const auto& [constantDecl, stringValues] : data.Constants) {
            const std::string enumConstantName = StringUtil::QualifiedName(*constantDecl);
            Printer_ << "case " << enumConstantName << ": {\n";
            Printer_.AddIndent();
            Printer_ << "return \"" << stringValues[0] << "\";\n";
            Printer_.DecreaseIndent();
            Printer_ << "}\n";
        }
        Printer_.DecreaseIndent();
        Printer_ << "}\n";

        Printer_.Unreachable();
        Printer_.DecreaseIndent();
        Printer_ << "}\n\n";
    }

private:
    Context& Ctx_;
    const EnumDatas& Datas_;
    NicePrinter Printer_;
};

} // namespace

void PrintEnumDatas(Context& ctx, const EnumDatas& enumDatas) {
    EnumDataPrinter printer{ctx, enumDatas};
    printer.Print();
}

} // namespace Waffle::EnumSerializer
