#include "collector.h"
#include "common.h"
#include "printer.h"

#include <lib/registry/registry.h>

using namespace Waffle;

void EnumSerializerDo(Context& ctx) {
    if (const auto enumDatas = EnumSerializer::CollectEnumDatas(ctx.AstContext); !enumDatas.empty()) {
        EnumSerializer::PrintEnumDatas(ctx, enumDatas);
    }
}

REGISTER_MODULE(enum_serializer, EnumSerializer::Commands(), EnumSerializerDo);
