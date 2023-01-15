#pragma once

#include <string_view>

namespace Waffle {

class SignalBase {
public:
    virtual ~SignalBase();
};

namespace Impl {

template<typename Type, typename... Args> std::string_view GetSignalName(void(Type::*ptr)(Args...));
template<typename Type, typename... Args> std::string_view GetSlotName(void(Type::*ptr)(Args...));

void Connect(const SignalBase* sender, std::string_view signal, const SignalBase* receiver, std::string_view slot);

} // namespace Impl

template<typename SenderType, typename ReceiverType, typename... Args>
void Connect(const SignalBase* sender, void(SenderType::*signal)(Args...),
             const SignalBase* receiver, void(ReceiverType::*slot)(Args...))
{
    Impl::Connect(sender, Impl::GetSignalName(signal), receiver, Impl::GetSlotName(slot));
}

} // namespace Waffle
