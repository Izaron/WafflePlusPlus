#include "collector.h"
#include "common.h"
#include "printer.h"

#include <lib/registry/registry.h>

namespace Waffle {

class JsonStructModule : public IModule {
public:
    std::string_view Name() const override;
    std::span<const std::string_view> Commands() const override;
    void Do(Context& context) const override;
};

} // namespace Waffle

using namespace Waffle;

std::string_view JsonStructModule::Name() const {
    return "json_struct";
}

std::span<const std::string_view> JsonStructModule::Commands() const {
    return JsonStruct::Commands();
}

void JsonStructModule::Do(Context& ctx) const {
    if (const auto structDatas = JsonStruct::CollectStructDatas(ctx.AstContext); !structDatas.empty()) {
        JsonStruct::PrintStructDatas(ctx, structDatas);
    }
}

REGISTER_MODULE(JsonStructModule);
