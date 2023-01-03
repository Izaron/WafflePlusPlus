#pragma once

#include <lib/file/i_file.h>

namespace Waffle {

struct Context {
    IFileManager& FileMgr;
};

class IModule {
public:
    virtual ~IModule() = default;
    virtual void Do(Context& context) = 0;
};

} // namespace Waffle
