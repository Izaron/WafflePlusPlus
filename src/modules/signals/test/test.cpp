#include <gtest/gtest.h>
#include "counter/counter.h"

TEST(Signals, Smoke) {
    model::Counter a, b;
    Waffle::Connect(&a, &model::Counter::ValueChanged,
                    &b, &model::Counter::SetValue);

    a.SetValue(12);
    ASSERT_EQ(a.Value(), 12);
    ASSERT_EQ(b.Value(), 12);

    b.SetValue(48);
    ASSERT_EQ(a.Value(), 12);
    ASSERT_EQ(b.Value(), 48);
}
