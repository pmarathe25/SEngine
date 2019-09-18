#include "archetype.hpp"
#include "world.hpp"
#include <STest.hpp>

using Stealth::Engine::Archetype, Stealth::Engine::World, Stealth::Engine::EntityID;

struct IFIDWorldFixture {
    using IF = Archetype<int32_t, float>;
    using ID = Archetype<int32_t, double>;
    World<IF, ID> world{};
};

STEST_F(IFIDWorldFixture, CanConstruct) { }

STEST_F(IFIDWorldFixture, CanCreateEntityInOrderComponents) {
    world.createEntity(0, 0.f);
}

STEST_F(IFIDWorldFixture, CanCreateEntityOutOfOrderComponents) {
    const int32_t x = 5;
    world.createEntity(0.f, x);
}

STEST_F(IFIDWorldFixture, FirstEntityHas0ID) {
    EntityID id = world.createEntity(0.f, 0);
    EXPECT_EQ(id, EntityID{0});
}

STEST_F(IFIDWorldFixture, EntityIDIsIncremented) {
    world.createEntity(0.f, 0);
    EntityID id = world.createEntity(0.f, 0);
    EXPECT_EQ(id, EntityID{1});
}

STEST_MAIN();
