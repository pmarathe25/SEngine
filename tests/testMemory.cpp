#include <Stealth/STest.hpp>
#include "rttbuffer.hpp"

using Stealth::Engine::RTTBuffer;

static constexpr size_t INITIAL_SIZE = 0;
static constexpr size_t INITIAL_CAPACITY = 5;

template <typename T>
class RTTBufFixture {
protected:
    RTTBuffer buf{RTTBuffer::create<T>(INITIAL_CAPACITY)};
};

class IntBufFixture : public RTTBufFixture<int> {
protected:
    void iota(size_t size = INITIAL_CAPACITY) {
        for (int i = 0; i < size; ++i) {
            buf.emplaceBack<int>(i);
        }
    }
};

STEST_F(IntBufFixture, Construct) { }

STEST_F(IntBufFixture, CopyConstruct) {
    iota();
    RTTBuffer buf2{buf};
    for (int i = 0; i < buf.size(); ++i) {
        EXPECT_EQ(buf.at<int>(i), buf2.at<int>(i));
    }
    EXPECT_EQ(buf.size(), buf2.size());
    EXPECT_EQ(buf.capacity(), buf2.capacity());

    EXPECT_EQ(buf.type(), buf2.type());
    EXPECT_EQ(buf.elementSize(), buf2.elementSize());
}

STEST_F(IntBufFixture, MoveConstruct) {
    iota();
    size_t size = buf.size();
    size_t capacity = buf.capacity();
    const auto& type = buf.type();
    size_t elementSize = buf.elementSize();

    RTTBuffer buf2{std::move(buf)};
    // Verify buf2
    for (int i = 0; i < buf.size(); ++i) {
        EXPECT_EQ(buf2.at<int>(i), i);
    }
    EXPECT_EQ(buf2.size(), size);
    EXPECT_EQ(buf2.capacity(), capacity);

    EXPECT_EQ(buf2.type(), type);
    EXPECT_EQ(buf2.elementSize(), elementSize);
    // Verify buf
    EXPECT_EQ(buf.size(), 0);
    EXPECT_EQ(buf.capacity(), 0);
    EXPECT_EQ(buf.data<int>(), nullptr);
}

// Type checks are disabled in non-debug builds for the sake of performance.
#ifdef RTT_BUF_ENABLE_TYPE_CHECKS
STEST_F(IntBufFixture, TypeChecks) {
    EXPECT_THROWS(buf.data<float>(), RTTBuffer::BadTypeCast);
}
#endif

STEST_F(IntBufFixture, BoundsChecks) {
    EXPECT_THROWS(buf.at<int>(buf.size()), std::out_of_range);
}

STEST_F(IntBufFixture, UncheckedAccessorDoesNotThrow) {
    buf.atUnchecked<int>(buf.size());
}

STEST_F(IntBufFixture, PushBack) {
    for (int i = 0; i < INITIAL_CAPACITY; ++i) {
        buf.pushBack(i);
    }
    for (int i = 0; i < buf.size(); ++i) {
        EXPECT_EQ(buf.at<int>(i), i);
    }
    EXPECT_EQ(buf.elementSize(), sizeof(int));
}

STEST_F(IntBufFixture, EmplaceBack) {
    for (int i = 0; i < INITIAL_CAPACITY; ++i) {
        buf.emplaceBack<int>(i);
    }
    for (int i = 0; i < buf.size(); ++i) {
        EXPECT_EQ(buf.at<int>(i), i);
    }
    EXPECT_EQ(buf.elementSize(), sizeof(int));
}

STEST_F(IntBufFixture, Reserve) {
    EXPECT_EQ(buf.size(), INITIAL_SIZE);
    EXPECT_EQ(buf.capacity(), INITIAL_CAPACITY);
    static constexpr size_t NEW_CAPACITY = INITIAL_CAPACITY * 2 + 1;
    buf.reserve(NEW_CAPACITY);
    EXPECT_EQ(buf.size(), INITIAL_SIZE);
    EXPECT_EQ(buf.capacity(), NEW_CAPACITY);
}

// TODO: Copy and move assignment ests

int main(int argc, const char* argv[]) {
    return static_cast<int>(RUN_STESTS(argc, argv));
}
