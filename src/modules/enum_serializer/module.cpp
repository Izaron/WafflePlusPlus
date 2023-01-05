#include "collector.h"
#include "common.h"
#include "module.h"
#include "printer.h"

#include <lib/module/registry.h>

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
