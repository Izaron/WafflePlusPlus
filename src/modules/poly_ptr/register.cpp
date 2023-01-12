#include "collector.h"
#include "common.h"
#include "printer.h"

using namespace Waffle;

static void Do(Context& ctx) {
    if (const auto decls = PolyPtr::Collect(ctx.AstContext); !decls.empty()) {
        PolyPtr::Print(ctx, decls);
    }
}

REGISTER_MODULE(poly_ptr, PolyPtr::Commands(), Do);
