#include "archetype.hpp"
#include "world.hpp"
#include <Stealth/STest.hpp>

using Stealth::Engine::Archetype, Stealth::Engine::World;

class IFIDWorldFixture {
protected:
    using IF = Archetype<int, float>;
    using ID = Archetype<int, double>;
    World<IF, ID> world{};
};

STEST_F(IFIDWorldFixture, CanConstruct) { }

STEST_MAIN();
