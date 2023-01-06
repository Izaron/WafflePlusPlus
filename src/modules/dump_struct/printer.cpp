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
            PrintRecordDecl(*data);
        }
        Printer_.CloseNamespace();
    }

private:
    void PrintRecordDecl(const clang::RecordDecl& decl) {
        const std::string typeName = StringUtil::QualifiedName(decl);
        Printer_ << "template<>\n";
        Printer_ << "std::string DumpStruct(const " << typeName << "& value) {\n";
        Printer_.AddIndent();
        Printer_ << "std::stringstream ss;\n";
        PrintAllFields(decl);
        Printer_ << "return ss.str();\n";
        Printer_.DecreaseIndent();
        Printer_ << "}\n\n";
    }

    void PrintAllFields(const clang::RecordDecl& decl) {
        StartPrintingLine();
        Printer_ << R"("{\n";)" << "\n";
        IncreasePrintIndent();
        for (const auto* field : decl.fields()) {
            PrintField(*field);
        }
        DecreasePrintIndent();
        StartPrintingLine();
        Printer_ << R"("}\n";)" << "\n";
    }

    void PrintField(const clang::FieldDecl& decl) {
        const std::string fieldName = decl.getNameAsString();
        const std::string typeName = GetTypeName(decl);
        const auto* type = decl.getType().getTypePtr();

        if (type->isArithmeticType() || type->isPointerType()) {
            StartPrintingLine();
            Printer_ << "\"" << fieldName << R"( = " << value.)" << fieldName << R"( << "\n";)" << "\n";
        }
        else if (typeName == "std::basic_string" || typeName == "std::basic_string_view") {
            StartPrintingLine();
            Printer_ << "\"" << fieldName << R"( = \"" << value.)" << fieldName << R"( << "\"\n";)" << "\n";
        }

        llvm::errs() << "Got type name \"" << typeName << "\"\n";
    }

    std::string GetTypeName(const clang::FieldDecl& decl) {
        if (const auto* typeDecl = decl.getType()->getAsTagDecl()) {
            return StringUtil::QualifiedName(*typeDecl);
        }
        return "";
    }

    void IncreasePrintIndent() {
        PrintIndent_ += 4;
    }

    void DecreasePrintIndent() {
        PrintIndent_ -= 4;
    }

    void StartPrintingLine() {
        Printer_ << "ss << ";
        if (PrintIndent_ > 0) {
            Printer_ << "\"";
            for (int i = 0; i < PrintIndent_; ++i) {
                Printer_ << " ";
            }
            Printer_ << "\" << ";
        }
    }

private:
    Context& Ctx_;
    const StructDatas& Datas_;
    NicePrinter Printer_;
    int PrintIndent_ = 0;
};

} // namespace

void PrintStructDatas(Context& ctx, const StructDatas& structDatas) {
    StructDataPrinter printer{ctx, structDatas};
    printer.Print();
}

} // namespace Waffle::DumpStruct
