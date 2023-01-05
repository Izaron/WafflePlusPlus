#pragma once

#include "file.h"

namespace Waffle {

class NicePrinter {
public:
    explicit NicePrinter(IFilePrinter& printer);

    void AddPreabmle(std::string_view source);
    void Include(std::string_view file);
    void NewLine(int count = 1);
    void OpenNamespace();
    void CloseNamespace();
    void Throw(std::string_view what);
    void ThrowString(std::string_view what);
    void AddIndent(int diff = 4);
    void DecreaseIndent(int diff = 4);

    NicePrinter& operator<<(std::string_view s);

private:
    int indent_ = 0;
    bool addIndent_ = true;
    IFilePrinter& proxyPrinter_;
};

} // namespace Waffle
