#include "printer.h"

#include <inja/inja.hpp>

#include <lib/file/file.h>
#include <lib/string_util/string_util.h>

namespace Waffle::DumpStruct {

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
        const std::string outputFile = StringUtil::InsertBeforeExt(inFile, "dump_struct");
        auto& printer = Ctx_.FileManager.GetOrCreateFilePrinter(outputFile);

        inja::json dataJson;
        dataJson["source_file"] = StringUtil::RemoveLastExt(inFile);
        for (const auto* data : Datas_) {
            auto& structJson = dataJson["structs"].emplace_back();
            structJson["name"] = StringUtil::QualifiedName(*data);

            auto& linesJson = structJson["lines"];
            linesJson = inja::json::array();
            LinesJson_ = &linesJson;
            AddFieldLines(*data);
        }

        inja::Environment env;
        env.set_trim_blocks(true);
        env.add_callback("indent", /*num_args=*/1, [](inja::Arguments& args) {
            int cnt = args.at(0)->get<int>();
            std::string indent = "";
            for (int i = 0; i < cnt; ++i) {
                indent += "    ";
            }
            return indent;
        });
        printer << env.render(TEMPLATE, dataJson);
    }

private:
    void AddFieldLines(const clang::RecordDecl& decl) {
        for (const auto* field : decl.fields()) {
            const std::string typeName = GetTypeName(*field);
            const auto* type = field->getType().getTypePtr();

            Path_.emplace_back(field->getNameAsString());

            inja::json lineJson;
            lineJson["path"] = Path_;

            if (type->isArithmeticType() || type->isPointerType()) {
                lineJson["type"] = "simple";
                LinesJson_->emplace_back(std::move(lineJson));
            } else if (typeName == "std::basic_string" || typeName == "std::basic_string_view") {
                lineJson["type"] = "string";
                LinesJson_->emplace_back(std::move(lineJson));
            } else if (type->isRecordType()) {
                lineJson["type"] = "struct";
                lineJson["struct_name"] = typeName;
                LinesJson_->emplace_back(std::move(lineJson));
                AddFieldLines(*type->getAsRecordDecl());
            }

            Path_.erase(Path_.end());
        }
    }

    std::string GetTypeName(const clang::FieldDecl& decl) {
        if (const auto* typeDecl = decl.getType()->getAsTagDecl()) {
            return StringUtil::QualifiedName(*typeDecl);
        }
        return "";
    }

private:
    Context& Ctx_;
    const StructDatas& Datas_;
    inja::json* LinesJson_ = nullptr;
    inja::json Path_;
};

} // namespace

void PrintStructDatas(Context& ctx, const StructDatas& structDatas) {
    StructDataPrinter printer{ctx, structDatas};
    printer.Print();
}

} // namespace Waffle::DumpStruct
