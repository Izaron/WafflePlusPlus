#include "collector.h"
#include "common.h"
#include "printer.h"

using namespace Waffle;

void JsonStructDo(Context& ctx) {
    if (const auto structDatas = JsonStruct::CollectStructDatas(ctx.AstContext); !structDatas.empty()) {
        JsonStruct::PrintStructDatas(ctx, structDatas);
    }
}

REGISTER_MODULE(json_struct, JsonStruct::Commands(), JsonStructDo);
