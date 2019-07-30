#include "meta/functions.hpp"
#include <Stealth/STest.hpp>

using Stealth::Engine::packContains, Stealth::Engine::packIsUnique, Stealth::Engine::packIsSubset, Stealth::Engine::packsAreEquivalent, Stealth::Engine::packIndex, Stealth::Engine::removeCVRef, Stealth::Engine::reorderPack;

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

namespace PackIndexTests {
    STEST(PackIndexCorrect) {
        EXPECT_EQ((packIndex<int32_t, float, char, double, uint8_t, int32_t>()), 4);
        EXPECT_EQ((packIndex<int32_t, int32_t, float, char, double, uint8_t>()), 0);
    }

    STEST(PackIndexInDuplicateReturnsFirstIndex) {
        EXPECT_EQ((packIndex<int32_t, float, char, int32_t, double, uint8_t, int32_t>()), 2);
        EXPECT_EQ((packIndex<char, float, double, int32_t, char, char, double, uint8_t, int32_t>()), 3);
    }
} // PackIndexTests

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

namespace reorderPackTests {
    STEST(CanReorderSimplePack) {
        std::tuple<int32_t, float, char> inp{51, 0.5f, 'h'};
        const auto out = reorderPack<char, float, int32_t>(inp);
        EXPECT_EQ(std::get<0>(out), std::get<2>(inp));
        EXPECT_EQ(std::get<1>(out), std::get<1>(inp));
        EXPECT_EQ(std::get<2>(out), std::get<0>(inp));
    }
} // reorderPackTests

STEST_MAIN();
