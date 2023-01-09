#include "collector.h"
#include "common.h"
#include "printer.h"

using namespace Waffle;

static void Do(Context& ctx) {
    if (const auto decls = RestController::Collect(ctx.AstContext); !decls.empty()) {
        RestController::Print(ctx, decls);
    }
}

REGISTER_MODULE(rest_controller, RestController::Commands(), Do);
