#pragma once

#include <lib/file/i_file.h>

#include <memory>

namespace Waffle {

struct Context {
    IFileManager& FileMgr;
};

class IModule {
public:
    virtual ~IModule() = default;
    virtual void Do(Context& context) = 0;
};

using IModulePtr = std::unique_ptr<IModule>;

} // namespace Waffle
