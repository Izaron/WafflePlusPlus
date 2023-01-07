#include "collector.h"
#include "common.h"
#include "printer.h"

#include <lib/registry/registry.h>

namespace Waffle {

class EnumSerializerModule : public IModule {
public:
    std::string_view Name() const override;
    std::span<const std::string_view> Commands() const override;
    void Do(Context& context) const override;
};

} // namespace Waffle

using namespace Waffle;

std::string_view EnumSerializerModule::Name() const {
    return "enum_serializer";
}

std::span<const std::string_view> EnumSerializerModule::Commands() const {
    return EnumSerializer::Commands();
}

void EnumSerializerModule::Do(Context& ctx) const {
    if (const auto enumDatas = EnumSerializer::CollectEnumDatas(ctx.AstContext); !enumDatas.empty()) {
        EnumSerializer::PrintEnumDatas(ctx, enumDatas);
    }
}

REGISTER_MODULE(EnumSerializerModule);
