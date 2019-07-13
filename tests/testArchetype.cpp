#include "archetype.hpp"
#include <Stealth/STest.hpp>

using Stealth::Engine::ArchetypeGroup;

class IntFloatGroupFixture {
protected:
    ArchetypeGroup<int, float> mGroup{};
};

STEST_F(IntFloatGroupFixture, CanConstruct) { }

STEST_MAIN();
