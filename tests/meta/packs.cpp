#include "meta/packs.hpp"
#include <STest.hpp>

using Stealth::Engine::Pack;

namespace PackContainsTests {
    STEST(PackDoesContainType) {
        EXPECT_TRUE((Pack<int32_t, float, double>::contains<int32_t>()));
        EXPECT_TRUE((Pack<char, uint8_t, int32_t, float, double>::contains<int32_t>()));
    }

    STEST(PackDoesNotContainType) {
        EXPECT_FALSE((Pack<int32_t, float, double>::contains<uint8_t>()));
    }

    STEST(EmptyPackIsAlwaysFalse) {
        EXPECT_FALSE((Pack<>::contains<uint8_t>()));
    }
} // PackContainsTests

namespace PackIndexTests {
    STEST(PackIndexCorrect) {
        EXPECT_EQ((Pack<float, char, double, uint8_t, int32_t>::index<int32_t>()), 4);
        EXPECT_EQ((Pack<int32_t, float, char, double, uint8_t>::index<int32_t>()), 0);
    }

    STEST(PackIndexInDuplicateReturnsFirstIndex) {
        EXPECT_EQ((Pack<float, char, int32_t, double, uint8_t, int32_t>::index<int32_t>()), 2);
        EXPECT_EQ((Pack<float, double, int32_t, char, char, double, uint8_t, int32_t>::index<char>()), 3);
    }
} // PackIndexTests

namespace PackIsUniqueTests {
    STEST(PackIsUnique) {
        EXPECT_TRUE((Pack<int32_t, float, double>::isUnique()));
    }

    STEST(PackIsNotUnique) {
        EXPECT_FALSE((Pack<float, float, double>::isUnique()));
        EXPECT_FALSE((Pack<int32_t, char, float, float, double>::isUnique()));
    }

    STEST(PackContainsOneElement) {
        EXPECT_TRUE((Pack<double>::isUnique()));
    }

    STEST(PackContainsZeroElements) {
        EXPECT_TRUE((Pack<>::isUnique()));
    }
} // PackIsUniqueTests

namespace PackIsSubsetTests {
    STEST(PackIsSubset) {
        EXPECT_TRUE((Pack<int32_t, float, double, char, uint8_t>::contains<int32_t, float, double>()));
    }

    STEST(PackIsNotSubset) {
        EXPECT_FALSE((Pack<int32_t, float, char, uint8_t>::contains<int32_t, float, double>()));
    }

    STEST(EmptyPackIsSubset) {
        EXPECT_TRUE((Pack<int32_t, float, char, uint8_t>::contains()));
    }
} // PackIsSubsetTests

namespace PacksAreEquivalentTests {
    STEST(PacksAreEquivalent) {
        EXPECT_TRUE((Pack<int32_t, float, double>::equivalent<float, double, int32_t>()));
    }

    STEST(PacksAreNotEquivalentElementsDifferent) {
        EXPECT_FALSE((Pack<int32_t, uint8_t, double>::equivalent<float, double, int32_t>()));
    }

    STEST(PacksAreNotEquivalentLengthDifferent) {
        EXPECT_FALSE((Pack<int32_t, char, float, double>::equivalent<float, double, int32_t>()));
    }

    STEST(EmptyPacksAreEquivalent) {
        EXPECT_TRUE((Pack<>::equivalent()));
    }
} // PacksAreEquivalentTests

struct IFPackFixture {
    using IFPack = Pack<int32_t, float>;
    IFPack pack{42, 0.f};
};

namespace PacksCanStoreValuesTests {
    STEST_F(IFPackFixture, CanConstruct) { }

    STEST_F(IFPackFixture, PackCanIndexByType) {
        EXPECT_EQ(pack.at<int32_t>(), 42);
        EXPECT_EQ(pack.at<float>(), 0.f);
    }

    STEST_F(IFPackFixture, PackCanIndexByNumber) {
        EXPECT_EQ(pack.at<0>(), 42);
        EXPECT_EQ(pack.at<1>(), 0.f);
    }
} // PacksCanStoreValuesTests

namespace PackVisitTests {
    STEST_F(IFPackFixture, CanVisitAllElements) {
        pack.visit([](const auto& value){
            if constexpr (std::is_same_v<decltype(value), const int32_t&>) {
                EXPECT_EQ(value, 42);
            } else if constexpr (std::is_same_v<decltype(value), const float&>) {
                EXPECT_EQ(value, 0.f);
            } else {
                throw std::runtime_error{"Unexpected type in Pack"};
            }
        });
    }
} // PackVisitTests

STEST_MAIN();
