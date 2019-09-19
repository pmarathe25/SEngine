#include "entity.hpp"
#include <STest.hpp>

using Stealth::Engine::EntityID;

STEST(NextWorks) {
    EntityID id{0};
    EntityID next = id.next();
    EXPECT_EQ(id, EntityID{0});
    EXPECT_EQ(next, EntityID{1});
}

STEST_MAIN();
