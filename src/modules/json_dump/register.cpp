#include "collector.h"
#include "common.h"
#include "printer.h"

using namespace Waffle;

void JsonDumpDo(Context& ctx) {
    if (const auto structDatas = JsonDump::CollectStructDatas(ctx.AstContext); !structDatas.empty()) {
        JsonDump::PrintStructDatas(ctx, structDatas);
    }
}

REGISTER_MODULE(json_dump, JsonDump::Commands(), JsonDumpDo);
