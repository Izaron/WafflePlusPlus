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
        Printer_.Include("sstream");
        Printer_.NewLine();
        Printer_.Include("waffle/modules/dump_struct/dump_struct.h");
        Printer_.NewLine();
        Printer_.IncludeLocal(StringUtil::RemoveLastExt(inFile));
        Printer_.NewLine();

        Printer_.OpenNamespace();
        for (const auto* data : Datas_) {
            PrintRecordDecl(data);
        }
        Printer_.CloseNamespace();
    }

private:
    void PrintRecordDecl(const clang::RecordDecl* decl) {
        const std::string typeName = StringUtil::QualifiedName(*decl);
        Printer_ << "template<>\n";
        Printer_ << "std::string DumpStruct(const " << typeName << "& value) {\n";
        Printer_.AddIndent();
        Printer_ << "std::stringstream ss;\n";
        for (const auto* field : decl->fields()) {
            PrintField(field);
        }
        Printer_ << "return ss.str();\n";
        Printer_.DecreaseIndent();
        Printer_ << "}\n\n";
    }

    void PrintField(const clang::FieldDecl* decl) {
        const std::string name = decl->getNameAsString();
        const auto* type = decl->getType().getTypePtr();
        if (type->isArithmeticType()) {
            Printer_ << R"(ss << ")" << name << R"( = " << value.)" << name << R"( << "\n";)" << "\n";
        }
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
