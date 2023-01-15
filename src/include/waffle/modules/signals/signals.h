#pragma once

#include <functional>

namespace Waffle {

class SignalBase {
public:
    virtual ~SignalBase();
};

template<typename SenderType, typename ReceiverType, typename... Args>
void Connect(const SignalBase* sender, void(SenderType::*signal)(Args...),
             const SignalBase* receiver, void(ReceiverType::*method)(Args...));

} // namespace Waffle
