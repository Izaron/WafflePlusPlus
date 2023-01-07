#include "collector.h"
#include "common.h"
#include "printer.h"

#include <lib/registry/registry.h>

namespace Waffle {

class DumpStructModule : public IModule {
public:
    std::string_view Name() const override;
    std::span<const std::string_view> Commands() const override;
    void Do(Context& context) const override;
};

} // namespace Waffle

using namespace Waffle;

std::string_view DumpStructModule::Name() const {
    return "dump_struct";
}

std::span<const std::string_view> DumpStructModule::Commands() const {
    return DumpStruct::Commands();
}

void DumpStructModule::Do(Context& ctx) const {
    if (const auto structDatas = DumpStruct::CollectStructDatas(ctx.AstContext); !structDatas.empty()) {
        DumpStruct::PrintStructDatas(ctx, structDatas);
    }
}

REGISTER_MODULE(DumpStructModule);
