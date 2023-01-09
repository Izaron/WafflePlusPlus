#include "collector.h"
#include "common.h"
#include "printer.h"

using namespace Waffle;

static void Do(Context& ctx) {
    if (const auto datas = DataClass::Collect(ctx.AstContext); !datas.empty()) {
        DataClass::Print(ctx, datas);
    }
}

REGISTER_MODULE(data_class, DataClass::Commands(), Do);
