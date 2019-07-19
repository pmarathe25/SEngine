#include <Stealth/STest.hpp>
#include "archetype.hpp"

using Stealth::Engine::ArchetypeGroup;

class IntFloatArchetypeFixture {
protected:
    ArchetypeGroup group{ArchetypeGroup::create<int32_t, float>()};
};

STEST_F(IntFloatArchetypeFixture, CanConstruct) { }

STEST_F(IntFloatArchetypeFixture, ContainsCorrectBuffers) {
    EXPECT_EQ(group.getComponents().at(std::type_index{typeid(int)}).type(), typeid(int));
    EXPECT_EQ(group.getComponents().at(std::type_index{typeid(float)}).type(), typeid(float));
}

STEST_MAIN();
