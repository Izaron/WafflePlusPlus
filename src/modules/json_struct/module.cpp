#include "collector.h"
#include "common.h"
#include "module.h"
#include "printer.h"

#include <lib/module/registry.h>

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
