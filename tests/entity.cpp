#include "entity.hpp"
#include <STest.hpp>

using Stealth::Engine::EntityID;

STEST(PreIncrementWorks) {
    EntityID id{0};
    ++id;
    EXPECT_EQ(static_cast<EntityID::UnderlyingType>(id), 1);
}

STEST(PostIncrementWorks) {
    EntityID id{0};
    EntityID old = id++;
    EXPECT_EQ(static_cast<EntityID::UnderlyingType>(old), 0);
    EXPECT_EQ(static_cast<EntityID::UnderlyingType>(id), 1);
}

STEST_MAIN();
