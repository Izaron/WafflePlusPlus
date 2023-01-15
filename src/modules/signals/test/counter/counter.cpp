#include "counter.h"

namespace model {

Counter::Counter()
    : Value_{0}
{
}

int Counter::Value() const {
    return Value_;
}

void Counter::SetValue(int value) {
    if (value != Value_) {
        Value_ = value;
        ValueChanged(value);
    }
}

} // namespace model
