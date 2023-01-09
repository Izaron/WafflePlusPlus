#include "collector.h"
#include "common.h"
#include "printer.h"

using namespace Waffle;

static void Do(Context& ctx) {
    if (const auto decls = GoogleMock::Collect(ctx.AstContext); !decls.empty()) {
        GoogleMock::Print(ctx, decls);
    }
}

REGISTER_MODULE(gmock, GoogleMock::Commands(), Do);
