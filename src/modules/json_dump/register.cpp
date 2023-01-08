#include "collector.h"
#include "common.h"
#include "printer.h"

using namespace Waffle;

void JsonDumpDo(Context& ctx) {
    if (const auto decls = JsonDump::CollectStructDecls(ctx.AstContext); !decls.empty()) {
        JsonDump::PrintStructDatas(ctx, decls);
    }
}

REGISTER_MODULE(json_dump, JsonDump::Commands(), JsonDumpDo);
