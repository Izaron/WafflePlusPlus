#include "collector.h"
#include "common.h"
#include "printer.h"

using namespace Waffle;

void DumpStructDo(Context& ctx) {
    if (const auto structDatas = DumpStruct::CollectStructDatas(ctx.AstContext); !structDatas.empty()) {
        DumpStruct::PrintStructDatas(ctx, structDatas);
    }
}

REGISTER_MODULE(dump_struct, DumpStruct::Commands(), DumpStructDo);
