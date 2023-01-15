#include <string>

#include <waffle/modules/signals/signals.h>

namespace model {

class Counter : public Waffle::SignalBase {
public:
    Counter();

    int Value() const;
    void SetValue(int value);

    // @signal
    void ValueChanged(int newValue);

private:
    int Value_;
};

} // namespace model
