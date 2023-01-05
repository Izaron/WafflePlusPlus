#include "printer.h"

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
        auto& printer = Ctx_.FileManager.GetOrCreateFilePrinter(StringUtil::InsertBeforeExt(inFile, INSERT_BEFORE_EXT));

        printer << "// Generated by the Waffle++ code generator. DO NOT EDIT!\n";
        printer << "// source: " << StringUtil::RemoveLastExt(inFile) << "\n\n";

        printer << "#include <waffle/modules/enum_serializer/enum_serializer.h>\n\n";
        printer << "#include <" << StringUtil::RemoveLastExt(Ctx_.InFile) << ">\n\n";

        printer << "namespace Waffle {\n\n";

        for (const auto& data : Datas_) {
            const std::string enumType = StringUtil::QualifiedName(*data.Decl);
            printer << "template<>\n";
            printer << enumType << " FromString<" << enumType << ">(std::string_view value) {\n";
            for (const auto& [constantDecl, stringValues] : data.Constants) {
                for (const auto& stringValue : stringValues) {
                    printer << "    " << "if (value == \"" << stringValue << "\") {\n";
                    printer << "    " << "    " << "return " << StringUtil::QualifiedName(*constantDecl) << ";\n";
                    printer << "    " << "}\n";
                }
            }
            printer << "    " << "// TODO: throw\n";
            printer << "}\n\n";
        }

        printer << "} // namespace Waffle\n";
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
