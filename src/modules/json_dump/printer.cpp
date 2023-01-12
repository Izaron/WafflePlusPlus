#include "printer.h"

#include <unordered_set>

#include <clang/AST/DeclTemplate.h>
#include <inja/inja.hpp>

#include <lib/file/file.h>
#include <lib/string_util/string_util.h>

namespace Waffle::JsonDump {

namespace {

#include "template.cpp.data"
const std::string TEMPLATE{(char*)template_cpp, template_cpp_len};

class Printer {
public:
    explicit Printer(Context& ctx, const StructDecls& decls)
        : Ctx_{ctx}
        , Decls_{decls}
    {
    }

    void Print() {
        const std::string_view inFile = StringUtil::AfterLastSlash(Ctx_.InFile);
        const std::string outputFile = StringUtil::InsertBeforeExt(inFile, "json_dump");
        auto& printer = Ctx_.FileManager.GetOrCreateFilePrinter(outputFile);

        DataJson_["source_file"] = StringUtil::RemoveLastExt(inFile);

        for (const auto& decl : Decls_) {
            AddStructJson(decl);
        }
        std::reverse(DataJson_["structs"].begin(), DataJson_["structs"].end());

        inja::Environment env;
        env.set_trim_blocks(true);
        printer << env.render(TEMPLATE, DataJson_);
    }

private:
    void AddStructJson(const StructDecl& decl) {
        auto& structJson = DataJson_["structs"].emplace_back();
        structJson["name"] = StringUtil::QualifiedName(*decl.Decl);

        auto& fieldsJson = structJson["fields"];
        for (const auto& field : decl.Fields) {
            auto& fieldJson = fieldsJson.emplace_back();
            auto& funcSuffixJson = fieldJson["func_suffix"];
            fieldJson["variable_name"] = field.VariableName;
            fieldJson["json_name"] = field.JsonName;

            if (JSON_ARRAY_TYPES.contains(field.TypeName)) {
                funcSuffixJson = "Array";
            } else if (JSON_NULLABLE_TYPES.contains(field.TypeName)) {
                funcSuffixJson = "Nullable";
            } else if (field.IsRecordType) {
                funcSuffixJson = "Object";
            } else {
                funcSuffixJson = "Primitive";
            }
        }
    }

private:
    Context& Ctx_;
    const StructDecls& Decls_;
    inja::json DataJson_;
};

} // namespace

void Print(Context& ctx, const StructDecls& decls) {
    Printer printer{ctx, decls};
    printer.Print();
}

} // namespace Waffle::JsonDump
