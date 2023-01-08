#include "printer.h"

#include <inja/inja.hpp>

#include <lib/file/file.h>
#include <lib/string_util/string_util.h>

namespace Waffle::JsonDump {

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
        const std::string outputFile = StringUtil::InsertBeforeExt(inFile, "json_dump");
        auto& printer = Ctx_.FileManager.GetOrCreateFilePrinter(outputFile);

        inja::json dataJson;
        dataJson["source_file"] = StringUtil::RemoveLastExt(inFile);
        for (const auto* data : Datas_) {
            auto& structJson = dataJson["structs"].emplace_back();
            structJson["name"] = StringUtil::QualifiedName(*data);

            auto& eventsJson = structJson["events"];
            eventsJson = inja::json::array();
            EventsJson_ = &eventsJson;
            AddEvents(*data);
        }

        inja::Environment env;
        env.set_trim_blocks(true);
        printer << env.render(TEMPLATE, dataJson);
    }

private:
    void AddEvents(const clang::RecordDecl& decl) {
    }

private:
    Context& Ctx_;
    const StructDatas& Datas_;
    inja::json* EventsJson_ = nullptr;
};

} // namespace

void PrintStructDatas(Context& ctx, const StructDatas& structDatas) {
    StructDataPrinter printer{ctx, structDatas};
    printer.Print();
}

} // namespace Waffle::JsonDump
