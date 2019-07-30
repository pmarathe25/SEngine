#include "meta/tuples.hpp"
#include <Stealth/STest.hpp>

using Stealth::Engine::tupleSelect;

namespace tupleSelect {
    STEST(CanReorderSimplePack) {
        std::tuple<int32_t, float, char> inp{51, 0.5f, 'h'};
        const auto out = tupleSelect<char, float, int32_t>(inp);
        EXPECT_EQ(std::get<0>(out), std::get<2>(inp));
        EXPECT_EQ(std::get<1>(out), std::get<1>(inp));
        EXPECT_EQ(std::get<2>(out), std::get<0>(inp));
    }
} // tupleSelect

STEST_MAIN();
