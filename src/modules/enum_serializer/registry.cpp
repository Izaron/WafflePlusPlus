#include "collector.h"
#include "common.h"
#include "printer.h"

#include <lib/registry/registry.h>

using namespace Waffle;

static void Do(Context& ctx) {
    if (const auto datas = EnumSerializer::Collect(ctx.AstContext); !datas.empty()) {
        EnumSerializer::Print(ctx, datas);
    }
}

REGISTER_MODULE(enum_serializer, EnumSerializer::Commands(), Do);
