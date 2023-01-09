#include "collector.h"
#include "common.h"
#include "printer.h"

using namespace Waffle;

static void Do(Context& ctx) {
    if (const auto decls = JsonDump::Collect(ctx.AstContext); !decls.empty()) {
        JsonDump::Print(ctx, decls);
    }
}

REGISTER_MODULE(json_dump, JsonDump::Commands(), Do);
