#include "memory/bidirectionalMap.hpp"
#include <STest.hpp>

using Stealth::Engine::BidirectionalMap;

using IFBiMap = BidirectionalMap<int32_t, float>;

class IFBiMapFixture : public IFBiMap {
public:
    IFBiMapFixture() {
        this->set(0, 1.0f);
    }
};

namespace InternalTests {
    STEST_F(IFBiMapFixture, CanInsertKeyValuePair) {
        EXPECT_EQ(*m1[0], 1.0f);
        EXPECT_EQ(*m2[1.0f], 0);
    }

    STEST_F(IFBiMapFixture, KeyAccessorWorks) {
        EXPECT_EQ(this->keyAt(1.0f), 0);
    }

    STEST_F(IFBiMapFixture, ValueAccessorWorks) {
        EXPECT_EQ(this->valueAt(0), 1.0f);
    }
} // InternalTests

STEST_MAIN();
