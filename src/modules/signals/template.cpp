// Generated by the Waffle++ code generator. DO NOT EDIT!
// source: {{ source_file }}

#include <algorithm>
#include <map>
#include <vector>

#include <waffle/modules/signals/signals.h>

#include "{{ source_file }}"

namespace Waffle {

namespace Impl {

// pointer to sender/receiver object and signal/slot string code
using StorageUnit = std::pair<const SignalBase*, std::string_view>;

inline std::map<StorageUnit, std::vector<StorageUnit>> SIGNALS_STORAGE;

## for struct in structs
## for types, signals in struct.grouped_signals
template<>
std::string_view GetSignalName(void({{ struct.qualified_name }}::*ptr)({{ types }})) {
## for signal in signals
    if (ptr == &{{ struct.qualified_name }}::{{ signal.name }}) {
        return "{{ struct.qualified_name }}::{{ signal.name }}";
    }
## endfor
    __builtin_unreachable();
}

## endfor
## for types, slots in struct.grouped_slots
template<>
std::string_view GetSlotName(void({{ struct.qualified_name }}::*ptr)({{ types }})) {
## for slot in slots
    if (ptr == &{{ struct.qualified_name }}::{{ slot.name }}) {
        return "{{ struct.qualified_name }}::{{ slot.name }}";
    }
## endfor
    __builtin_unreachable();
}

## endfor
## endfor
void Connect(const SignalBase* sender, std::string_view signal, const SignalBase* receiver, std::string_view slot) {
    auto& vec = SIGNALS_STORAGE[{sender, signal}];
    vec.emplace_back(receiver, slot);
}

template<typename... Args>
void CallSlot(const SignalBase* constReceiver, std::string_view slot, Args... args);

## for struct in structs
## for types, slots in struct.grouped_slots
template<>
void CallSlot(const SignalBase* constReceiver, std::string_view slot, {{ first(slots).signature }}) {
    SignalBase* receiver = const_cast<SignalBase*>(constReceiver);
## for slot in slots
    if (slot == "{{ struct.qualified_name }}::{{ slot.name }}") {
        dynamic_cast<{{ struct.qualified_name }}*>(receiver)->{{ slot.name }}({{ slot.args }});
    }
## endfor
}

## endfor
## endfor

} // namespace Impl

SignalBase::~SignalBase() {
    // remove as a signal
    std::erase_if(Impl::SIGNALS_STORAGE, [this](const auto& item) {
        const auto& key = item.first;
        const auto* ptr = key.first;
        return ptr == this;
    });

    // remove as a slot
    for (auto& iter : Impl::SIGNALS_STORAGE) {
        auto& vec = iter.second;
        vec.erase(std::remove_if(vec.begin(), vec.end(), [this](const auto& item) {
            return item.first == this;
        }));
    }
}

} // namespace Waffle
## for struct in structs
## for signal in struct.signals

void {{ struct.qualified_name }}::{{ signal.name }}({{ signal.signature }}) {
    auto& vec = Waffle::Impl::SIGNALS_STORAGE[{this, "{{ struct.qualified_name }}::{{ signal.name }}"}];
    for (const auto& [receiver, slot] : vec) {
        Waffle::Impl::CallSlot(receiver, slot, {{ signal.args }});
    }
}
## endfor
## endfor
