#include "collector.h"
#include "common.h"
#include "printer.h"

using namespace Waffle;

static void Do(Context& ctx) {
    if (const auto decls = Signals::Collect(ctx.AstContext); !decls.empty()) {
        Signals::Print(ctx, decls);
    }
}

REGISTER_MODULE(signals, Signals::Commands(), Do);
