#include <Stealth/STest.hpp>
#include "rttbuffer.hpp"

using Stealth::Engine::RTTBuffer;

template <typename T>
class RTTBufFixture {
protected:
    static constexpr size_t INITIAL_SIZE = 5;
    RTTBuffer buf{RTTBuffer::create<T>(INITIAL_SIZE)};
};

using IntBufFixture = RTTBufFixture<int>;

STEST_F(IntBufFixture, CanConstruct) { }

// Type checks are disabled in non-debug builds for the sake of performance.
#ifdef RTT_BUF_ENABLE_TYPE_CHECKS
STEST_F(IntBufFixture, TypeChecksWork) {
    EXPECT_THROWS(buf.at<float>(0), RTTBuffer::BadTypeCast);
}
#endif

STEST_F(IntBufFixture, BoundsChecksWork) {
    EXPECT_THROWS(buf.at<int>(buf.size()), std::out_of_range);
}

STEST_F(IntBufFixture, UncheckedAccessorDoesNotThrow) {
    buf.atUnchecked<int>(buf.size());
}

STEST_F(IntBufFixture, CanSetAndGetMemory) {
    for (int i = 0; i < buf.size(); ++i) {
        buf.at<int>(i) = i;
    }
    for (int i = 0; i < buf.size(); ++i) {
        EXPECT_EQ(buf.at<int>(i), i);
    }
    EXPECT_EQ(buf.elementSize(), sizeof(int));
}

STEST_F(IntBufFixture, CanResize) {
    EXPECT_EQ(buf.size(), INITIAL_SIZE);
    static constexpr size_t NEW_SIZE = INITIAL_SIZE * 2 + 1;
    buf.resize(NEW_SIZE);
    EXPECT_EQ(buf.size(), NEW_SIZE);
}

int main(int argc, const char* argv[]) {
    return static_cast<int>(RUN_STESTS(argc, argv));
}
