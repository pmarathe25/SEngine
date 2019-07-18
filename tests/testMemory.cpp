#include <Stealth/STest.hpp>
#include "rttbuffer.hpp"

using Stealth::Engine::RTTBuffer;

static constexpr size_t DEFAULTL_SIZE = 0;
static constexpr size_t DEFAULT_CAPACITY = 5;

template <typename T>
class RTTBufFixture {
protected:
    RTTBuffer buf{RTTBuffer::create<T>(DEFAULT_CAPACITY)};
};

class IntBufFixture : public RTTBufFixture<int> {
protected:
    void iota(size_t size = DEFAULT_CAPACITY) {
        for (int i = 0; i < size; ++i) {
            buf.emplaceBack<int>(i);
        }
    }
};

namespace ConstructorAssignmentTests {
    STEST_F(IntBufFixture, Construct) { }

    void checkBufferEquality(const RTTBuffer& lhs, const RTTBuffer& rhs) {
        for (int i = 0; i < lhs.size(); ++i) {
            EXPECT_EQ(lhs.at<int>(i), rhs.at<int>(i));
        }
        EXPECT_EQ(lhs.size(), rhs.size());
        EXPECT_EQ(lhs.capacity(), rhs.capacity());

        EXPECT_EQ(lhs.type(), rhs.type());
        EXPECT_EQ(lhs.elementSize(), rhs.elementSize());
    }

    STEST_F(IntBufFixture, CopyConstruct) {
        iota();
        RTTBuffer buf2{buf};
        checkBufferEquality(buf, buf2);
    }

    STEST_F(IntBufFixture, CopyAssign) {
        iota();
        RTTBuffer buf2 = buf;
        checkBufferEquality(buf, buf2);
    }

    void checkBufferMoved(const RTTBuffer& oldBuffer, const RTTBuffer& newBuffer, size_t oldSize, size_t oldCapacity, const std::type_info& oldType, size_t oldElementSize) {
        // Verify the new buffer is correct.
        for (int i = 0; i < newBuffer.size(); ++i) {
            EXPECT_EQ(newBuffer.at<int>(i), i);
        }
        EXPECT_EQ(newBuffer.size(), oldSize);
        EXPECT_EQ(newBuffer.capacity(), oldCapacity);

        EXPECT_EQ(newBuffer.type(), oldType);
        EXPECT_EQ(newBuffer.elementSize(), oldElementSize);
        // Verify buf
        EXPECT_EQ(oldBuffer.size(), 0);
        EXPECT_EQ(oldBuffer.capacity(), 0);
        EXPECT_EQ(oldBuffer.data<int>(), nullptr);
    }

    STEST_F(IntBufFixture, MoveConstruct) {
        iota();
        size_t size = buf.size();
        size_t capacity = buf.capacity();
        const auto& type = buf.type();
        size_t elementSize = buf.elementSize();

        RTTBuffer buf2{std::move(buf)};
        checkBufferMoved(buf, buf2, size, capacity, type, elementSize);
    }

    STEST_F(IntBufFixture, MoveAssign) {
        iota();
        size_t size = buf.size();
        size_t capacity = buf.capacity();
        const auto& type = buf.type();
        size_t elementSize = buf.elementSize();

        RTTBuffer buf2 = std::move(buf);
        checkBufferMoved(buf, buf2, size, capacity, type, elementSize);
    }
} // ConstructorAssignmentTests

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
    for (int i = 0; i < DEFAULT_CAPACITY; ++i) {
        buf.pushBack(i);
    }
    for (int i = 0; i < buf.size(); ++i) {
        EXPECT_EQ(buf.at<int>(i), i);
    }
    EXPECT_EQ(buf.elementSize(), sizeof(int));
}

STEST_F(IntBufFixture, EmplaceBack) {
    for (int i = 0; i < DEFAULT_CAPACITY; ++i) {
        buf.emplaceBack<int>(i);
    }
    for (int i = 0; i < buf.size(); ++i) {
        EXPECT_EQ(buf.at<int>(i), i);
    }
    EXPECT_EQ(buf.elementSize(), sizeof(int));
}

STEST(EmplaceBackDoesNotCauseExcessiveReallocation) {
    // To emplace 8 elements, only 3 reallocations should occur -> 0->2->4->8
    RTTBuffer buf{RTTBuffer::create<int>(0)};
    static constexpr int DESIRED_CAPACITY = 8;

    size_t currentCapacity{buf.capacity()};
    size_t numReallocations{0};

    for (int i = 0; i < DESIRED_CAPACITY; ++i) {
        buf.emplaceBack<int>(i);
        if (buf.capacity() != currentCapacity) {
            currentCapacity = buf.capacity();
            ++numReallocations;
        }
    }

    EXPECT_EQ(numReallocations, 3);
}

STEST_F(IntBufFixture, Reserve) {
    EXPECT_EQ(buf.size(), DEFAULTL_SIZE);
    EXPECT_EQ(buf.capacity(), DEFAULT_CAPACITY);
    static constexpr size_t NEW_CAPACITY = DEFAULT_CAPACITY * 2 + 1;
    buf.reserve(NEW_CAPACITY);
    EXPECT_EQ(buf.size(), DEFAULTL_SIZE);
    EXPECT_EQ(buf.capacity(), NEW_CAPACITY);
}

// TODO: Copy and move assignment tests

STEST_MAIN();
