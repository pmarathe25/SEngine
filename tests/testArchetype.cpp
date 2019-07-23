#include "archetype.hpp"
#include <Stealth/STest.hpp>

using Stealth::Engine::ArchetypeGroup, Stealth::Engine::Archetype;

STEST(IdenticalArchetypesAreSame) {
    EXPECT_TRUE((std::is_same_v<Archetype<int32_t, float>, Archetype<int32_t, float>>));
}

STEST(DifferentlyOrderedArchetypesAreSame) {
    EXPECT_TRUE((std::is_same_v<Archetype<float, int32_t>, Archetype<int32_t, float>>));
}

STEST(DifferentArchetypesAreDifferent) {
    EXPECT_FALSE((std::is_same_v<Archetype<float, uint8_t>, Archetype<float, int32_t>>));
}

STEST(ArchetypeContainsCorrectElements) {
    using IFDC = Archetype<int32_t, float, double, char>;
    EXPECT_TRUE(IFDC::contains<int32_t>);
    EXPECT_TRUE(IFDC::contains<float>);
    EXPECT_TRUE(IFDC::contains<double>);
    EXPECT_TRUE(IFDC::contains<char>);
}

class IntFloatGroupFixture {
protected:
    ArchetypeGroup<int, float> mGroup{};
};

STEST_F(IntFloatGroupFixture, CanConstruct) { }

STEST_MAIN();
