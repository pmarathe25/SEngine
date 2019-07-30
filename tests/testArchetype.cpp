#include "archetype.hpp"
#include "common.hpp"
#include <Stealth/STest.hpp>

using Stealth::Engine::Archetype;
using common::ArchetypeInternalFixture;
using Stealth::Engine::removeCVRef;

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
    EXPECT_TRUE(IFDC::containsType<int32_t>());
    EXPECT_TRUE(IFDC::containsType<float>());
    EXPECT_TRUE(IFDC::containsType<double>());
    EXPECT_TRUE(IFDC::containsType<char>());
}

class IntFloatFixture {
protected:
    common::IFArchetype archetype{};
};

STEST_F(IntFloatFixture, CanConstruct) { }

STEST_F(IntFloatFixture, CanGetVectors) {
    EXPECT_TRUE((std::is_same_v<removeCVRef<decltype(archetype.storage<int32_t>())>, common::IFArchetype::StorageType<int32_t>>));
    EXPECT_EQ(archetype.storage<int32_t>().size(), 0);
}

// Internal tests
STEST_F(ArchetypeInternalFixture, CanAddComponentsInOrder) {
    constexpr int32_t i = 10;
    constexpr float f = 0.125f;
    this->addComponents(i, f);
    EXPECT_EQ(intStorage.size(), 1);
    EXPECT_EQ(intStorage.at(0), i);
    EXPECT_EQ(floatStorage.size(), 1);
    EXPECT_EQ(floatStorage.at(0), f);
}

STEST_F(ArchetypeInternalFixture, CanAddComponentsOutOfOrder) {
    constexpr int32_t i = 10;
    constexpr float f = 0.125f;
    this->addComponents(f, i);
    EXPECT_EQ(intStorage.size(), 1);
    EXPECT_EQ(intStorage.at(0), i);
    EXPECT_EQ(floatStorage.size(), 1);
    EXPECT_EQ(floatStorage.at(0), f);
}


STEST_MAIN();
