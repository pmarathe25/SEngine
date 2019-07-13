#include "meta/functions.hpp"
#include <Stealth/STest.hpp>

using Stealth::Engine::packContains, Stealth::Engine::packIsUnique, Stealth::Engine::packIsSubset, Stealth::Engine::packsAreEquivalent;

namespace PackContainsTests {
    STEST(PackDoesContainType) {
        EXPECT_TRUE((packContains<int32_t, int32_t, float, double>()));
        EXPECT_TRUE((packContains<int32_t, char, uint8_t, int32_t, float, double>()));
    }

    STEST(PackDoesNotContainType) {
        EXPECT_FALSE((packContains<uint8_t, int32_t, float, double>()));
    }

    STEST(EmptyPackIsAlwaysFalse) {
        EXPECT_FALSE((packContains<uint8_t>()));
    }
} // PackContainsTests

namespace PackIsUniqueTests {
    STEST(PackIsUnique) {
        EXPECT_TRUE((packIsUnique<int32_t, float, double>()));
    }

    STEST(PackIsNotUnique) {
        EXPECT_FALSE((packIsUnique<float, float, double>()));
        EXPECT_FALSE((packIsUnique<int32_t, char, float, float, double>()));
    }

    STEST(PackContainsOneElement) {
        EXPECT_TRUE((packIsUnique<double>()));
    }

    STEST(PackContainsZeroElements) {
        EXPECT_TRUE((packIsUnique<>()));
    }
} // PackIsUniqueTests

namespace PackIsSubsetTests {
    STEST(PackIsSubset) {
        EXPECT_TRUE((packIsSubset(std::tuple<int32_t, float, double>{}, std::tuple<int32_t, float, double, char, uint8_t>{})))
    }

    STEST(PackIsNotSubset) {
        EXPECT_FALSE((packIsSubset(std::tuple<int32_t, float, double>{}, std::tuple<int32_t, float, char, uint8_t>{})))
    }

    STEST(EmptyPackIsSubset) {
        EXPECT_TRUE((packIsSubset(std::tuple{}, std::tuple<int32_t, float, char, uint8_t>{})))
    }
} // PackIsSubsetTests

namespace PacksAreEquivalentTests {
    STEST(PacksAreEquivalent) {
        EXPECT_TRUE((packsAreEquivalent(std::tuple<int32_t, float, double>{}, std::tuple<float, double, int32_t>{})));
    }

    STEST(PacksAreNotEquivalentElementsDifferent) {
        EXPECT_FALSE((packsAreEquivalent(std::tuple<int32_t, uint8_t, double>{}, std::tuple<float, double, int32_t>{})));
    }

    STEST(PacksAreNotEquivalentLengthDifferent) {
        EXPECT_FALSE((packsAreEquivalent(std::tuple<int32_t, char, float, double>{}, std::tuple<float, double, int32_t>{})));
    }

    STEST(EmptyPacksAreEquivalent) {
        EXPECT_TRUE((packsAreEquivalent(std::tuple{}, std::tuple{})));
    }
} // PacksAreEquivalentTests

STEST_MAIN();
