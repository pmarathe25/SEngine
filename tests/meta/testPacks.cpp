#include "meta/packs.hpp"
#include <Stealth/STest.hpp>

using Stealth::Engine::removeCVRef, Stealth::Engine::ParameterPack;

STEST(RemoveCVRefWorks) {
    EXPECT_TRUE((std::is_same_v<removeCVRef<const int32_t&&>, int32_t>));
    EXPECT_TRUE((std::is_same_v<removeCVRef<const int32_t&>, int32_t>));
    EXPECT_TRUE((std::is_same_v<removeCVRef<const int32_t>, int32_t>));
    EXPECT_TRUE((std::is_same_v<removeCVRef<int32_t&&>, int32_t>));
    EXPECT_TRUE((std::is_same_v<removeCVRef<int32_t&>, int32_t>));
    EXPECT_TRUE((std::is_same_v<removeCVRef<int32_t>, int32_t>));
}

namespace PackContainsTests {
    STEST(PackDoesContainType) {
        EXPECT_TRUE((ParameterPack<int32_t, float, double>::contains<int32_t>()));
        EXPECT_TRUE((ParameterPack<char, uint8_t, int32_t, float, double>::contains<int32_t>()));
    }

    STEST(PackDoesNotContainType) {
        EXPECT_FALSE((ParameterPack<int32_t, float, double>::contains<uint8_t>()));
    }

    STEST(EmptyPackIsAlwaysFalse) {
        EXPECT_FALSE((ParameterPack<>::contains<uint8_t>()));
    }
} // PackContainsTests

namespace PackIndexTests {
    STEST(PackIndexCorrect) {
        EXPECT_EQ((ParameterPack<float, char, double, uint8_t, int32_t>::index<int32_t>()), 4);
        EXPECT_EQ((ParameterPack<int32_t, float, char, double, uint8_t>::index<int32_t>()), 0);
    }

    STEST(PackIndexInDuplicateReturnsFirstIndex) {
        EXPECT_EQ((ParameterPack<float, char, int32_t, double, uint8_t, int32_t>::index<int32_t>()), 2);
        EXPECT_EQ((ParameterPack<float, double, int32_t, char, char, double, uint8_t, int32_t>::index<char>()), 3);
    }
} // PackIndexTests

namespace PackIsUniqueTests {
    STEST(PackIsUnique) {
        EXPECT_TRUE((ParameterPack<int32_t, float, double>::isUnique()));
    }

    STEST(PackIsNotUnique) {
        EXPECT_FALSE((ParameterPack<float, float, double>::isUnique()));
        EXPECT_FALSE((ParameterPack<int32_t, char, float, float, double>::isUnique()));
    }

    STEST(PackContainsOneElement) {
        EXPECT_TRUE((ParameterPack<double>::isUnique()));
    }

    STEST(PackContainsZeroElements) {
        EXPECT_TRUE((ParameterPack<>::isUnique()));
    }
} // PackIsUniqueTests

namespace PackIsSubsetTests {
    STEST(PackIsSubset) {
        EXPECT_TRUE((ParameterPack<int32_t, float, double, char, uint8_t>::contains<int32_t, float, double>()));
    }

    STEST(PackIsNotSubset) {
        EXPECT_FALSE((ParameterPack<int32_t, float, char, uint8_t>::contains<int32_t, float, double>()));
    }

    STEST(EmptyPackIsSubset) {
        EXPECT_TRUE((ParameterPack<int32_t, float, char, uint8_t>::contains()));
    }
} // PackIsSubsetTests

namespace PacksAreEquivalentTests {
    STEST(PacksAreEquivalent) {
        EXPECT_TRUE((ParameterPack<int32_t, float, double>::equivalent<float, double, int32_t>()));
    }

    STEST(PacksAreNotEquivalentElementsDifferent) {
        EXPECT_FALSE((ParameterPack<int32_t, uint8_t, double>::equivalent<float, double, int32_t>()));
    }

    STEST(PacksAreNotEquivalentLengthDifferent) {
        EXPECT_FALSE((ParameterPack<int32_t, char, float, double>::equivalent<float, double, int32_t>()));
    }

    STEST(EmptyPacksAreEquivalent) {
        EXPECT_TRUE((ParameterPack<>::equivalent()));
    }
} // PacksAreEquivalentTests

STEST_MAIN();
