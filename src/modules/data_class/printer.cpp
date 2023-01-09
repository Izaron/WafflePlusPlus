#include "printer.h"

#include <clang/AST/ASTContext.h>
#include <clang/AST/PrettyPrinter.h>

#include <inja/inja.hpp>

#include <lib/file/file.h>
#include <lib/string_util/string_util.h>

namespace Waffle::DataClass {

namespace {

#include "template.cpp.data"
const std::string TEMPLATE{(char*)template_cpp, template_cpp_len};

class Printer {
public:
    explicit Printer(Context& ctx, const ClassDatas& datas)
        : Ctx_{ctx}
        , Datas_{datas}
    {
    }

    void Print() {
        const std::string_view inFile = StringUtil::AfterLastSlash(Ctx_.InFile);
        const std::string changedInFile = StringUtil::InsertBeforeExt(inFile, "data_class");
        const std::string outputFile = std::string{StringUtil::RemoveLastExt(changedInFile)} + ".h";
        auto& printer = Ctx_.FileManager.GetOrCreateFilePrinter(outputFile);

        for (const auto& data : Datas_) {
            GeneratedTypeNames_.emplace(StringUtil::QualifiedName(*data.Decl), data.Name);
        }

        DataJson_["source_file"] = StringUtil::RemoveLastExt(inFile);
        for (const auto& data : Datas_) {
            AddDataClass(data);
        }

        llvm::errs() << DataJson_.dump(4) << "\n";

        inja::Environment env;
        env.set_trim_blocks(true);

        constexpr auto getFieldName = [](inja::Arguments& args) { return args[0]->get<std::string>(); };
        constexpr auto getFieldNameUpper = [getFieldName](inja::Arguments& args) {
            auto name = getFieldName(args);
            name[0] = std::toupper(name[0]);
            return name;
        };
        constexpr auto privateField = [getFieldName](inja::Arguments& args) {
            return getFieldName(args) + "_";
        };
        constexpr auto setter = [getFieldNameUpper](inja::Arguments& args) {
            return "Set" + getFieldNameUpper(args);
        };
        constexpr auto getter = [getFieldNameUpper](inja::Arguments& args) {
            return "Get" + getFieldNameUpper(args);
        };

        env.add_callback("privateField", /*num_args=*/1, privateField);
        env.add_callback("setter", /*num_args=*/1, setter);
        env.add_callback("getter", /*num_args=*/1, getter);

        printer << env.render(TEMPLATE, DataJson_);
    }

private:
    void AddDataClass(const ClassData& data) {
        auto& classJson = DataJson_["classes"].emplace_back();
        classJson["name"] = data.Name;

        const auto& decl = *data.Decl;
        classJson["stub_name"] = StringUtil::QualifiedName(*data.Decl);

        for (const auto field : decl.fields()) {
            auto& fieldJson = classJson["fields"].emplace_back();
            fieldJson["name"] = field->getNameAsString();
            fieldJson["type"] = GetGeneratedTypeName(*field);
            fieldJson["is_light_type"] = IsLightType(*field->getType().getTypePtr());
        }
    }

    std::string GetGeneratedTypeName(const clang::FieldDecl& field) {
        static const auto printingPolicy = std::invoke([this]{
            auto p = Ctx_.AstContext.getPrintingPolicy();
            p.SuppressTagKeyword = true;
            return p;
        });
        std::string name = field.getType().getAsString(printingPolicy);
        if (const auto iter = GeneratedTypeNames_.find(name); iter != GeneratedTypeNames_.end()) {
            return iter->second;
        }
        return name;
    }

    bool IsLightType(const clang::Type& type) {
        // is void/nullptr_t/enum/integers/floating
        return type.isFundamentalType();
    }

private:
    Context& Ctx_;
    const ClassDatas& Datas_;
    std::unordered_map<std::string, std::string> GeneratedTypeNames_;
    inja::json DataJson_;
};

} // namespace

void Print(Context& ctx, const ClassDatas& datas) {
    Printer printer{ctx, datas};
    printer.Print();
}

} // namespace Waffle::DataClass
