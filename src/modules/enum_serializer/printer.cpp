#include "printer.h"

#include <inja/inja.hpp>

#include <lib/file/nice_printer.h>
#include <lib/string_util/string_util.h>

namespace Waffle::EnumSerializer {

namespace {

constexpr std::string_view INSERT_BEFORE_EXT = "enum_serializer";

#include "template.cpp.data"
const std::string TEMPLATE = reinterpret_cast<char*>(template_cpp);

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

        inja::json data;
        data["source_file"] = StringUtil::RemoveLastExt(inFile);
        Printer_ << inja::render(TEMPLATE, data);

        Printer_.OpenNamespace();
        for (const auto& data : Datas_) {
            PrintEnumData(data);
        }
        Printer_.CloseNamespace();
    }

private:
    void PrintEnumData(const EnumData& data) {
        PrintFromString(data);
        PrintFromStringOrDefault(data);
        PrintToString(data);
        PrintGetAllEnumValues(data);
    }

    void PrintFromStringMainBody(const EnumData& data) {
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
    }

    void PrintFromString(const EnumData& data) {
        const std::string enumType = StringUtil::QualifiedName(*data.Decl);
        Printer_ << "template<>\n";
        Printer_ << enumType << " FromString<" << enumType << ">(std::string_view value) {\n";
        Printer_.AddIndent();
        PrintFromStringMainBody(data);
        Printer_.Throw(R"("Can't parse value \"" + std::string{value} + "\" to enum type \")" + enumType + R"(\"")");
        Printer_.DecreaseIndent();
        Printer_ << "}\n\n";
    }

    void PrintFromStringOrDefault(const EnumData& data) {
        const std::string enumType = StringUtil::QualifiedName(*data.Decl);
        Printer_ << "template<>\n";
        Printer_ << enumType << " FromStringOrDefault<" << enumType << ">(std::string_view value, " << enumType << " defaultResult) {\n";
        Printer_.AddIndent();
        PrintFromStringMainBody(data);
        Printer_ << "return defaultResult;\n";
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

    void PrintGetAllEnumValues(const EnumData& data) {
        const std::string enumType = StringUtil::QualifiedName(*data.Decl);
        Printer_ << "template<>\n";
        Printer_ << "std::span<const " << enumType << "> GetAllEnumValues() {\n";
        Printer_.AddIndent();
        Printer_ << "static const std::vector<" << enumType << "> values{\n";
        Printer_.AddIndent();
        for (const auto& [constantDecl, _] : data.Constants) {
            const std::string enumConstantName = StringUtil::QualifiedName(*constantDecl);
            Printer_ << enumConstantName << ",\n";
        }
        Printer_.DecreaseIndent();
        Printer_ << "};\n";
        Printer_ << "return values;\n";
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
