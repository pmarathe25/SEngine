#include "archetype.hpp"
#include "world.hpp"
#include <Stealth/STest.hpp>

using Stealth::Engine::Archetype, Stealth::Engine::World;

class IFIDWorldFixture {
protected:
    using IF = Archetype<int32_t, float>;
    using ID = Archetype<int32_t, double>;
    World<IF, ID> world{};
};

STEST_F(IFIDWorldFixture, CanConstruct) { }

STEST_MAIN();
