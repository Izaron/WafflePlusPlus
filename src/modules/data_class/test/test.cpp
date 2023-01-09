#include <gtest/gtest.h>
#include "mountains/mountains.data_class.h"

using namespace Waffle;

TEST(DataClass, Mountains) {
    LatLon pos{27.988056, 86.925278};
    ASSERT_EQ(pos.GetLatitude(), 27.988056);
    ASSERT_EQ(pos.GetLongitude(), 86.925278);

    Mountain m("Everest", "Nepal", std::move(pos), 8848);
    ASSERT_TRUE(m.GetName().has_value());
    ASSERT_STREQ(m.GetName()->data(), "Everest");
    ASSERT_STREQ(m.GetCountry().data(), "Nepal");
    ASSERT_EQ(m.GetPeak(), 8848);
}
