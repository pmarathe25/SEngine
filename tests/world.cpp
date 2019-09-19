#include "archetype.hpp"
#include "world.hpp"
#include <STest.hpp>

using Stealth::Engine::Archetype, Stealth::Engine::World, Stealth::Engine::EntityID;

using IF = Archetype<int32_t, float>;
using ID = Archetype<int32_t, double>;
struct IFIDWorldFixture : public World<IF, ID> { };

STEST_F(IFIDWorldFixture, CanConstruct) { }

STEST_F(IFIDWorldFixture, CanCreateEntityInOrderComponents) {
    EXPECT_EQ(mArchetypes.at<IF>().size(), 0);
    this->createEntity(0, 0.f);
    EXPECT_EQ(mArchetypes.at<IF>().size(), 1);
}

STEST_F(IFIDWorldFixture, CanCreateEntityOutOfOrderComponents) {
    EXPECT_EQ(mArchetypes.at<IF>().size(), 0);
    const int32_t x = 5;
    this->createEntity(0.f, x);
    EXPECT_EQ(mArchetypes.at<IF>().size(), 1);
}

struct PopulatedWorldFixture : public IFIDWorldFixture {
    PopulatedWorldFixture() : firstEntity{this->createEntity(0.f, 0).id()} { }

    EntityID firstEntity;
};

STEST_F(PopulatedWorldFixture, FirstEntityHas0ID) {
    EXPECT_EQ(firstEntity, EntityID{0});
}

STEST_F(PopulatedWorldFixture, EntityIDIsIncremented) {
    EntityID id = this->createEntity(0.f, 0).id();
    EXPECT_EQ(id, EntityID{1});
}

STEST_F(PopulatedWorldFixture, CanDestroyEntity) {
    // TODO: FIXME:
    this->destroyEntity(firstEntity);
    EXPECT_EQ(mArchetypes.at<IF>().size(), 0);

}

STEST_MAIN();
