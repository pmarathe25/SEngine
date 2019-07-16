#include <Stealth/STest.hpp>
#include "rttbuffer.hpp"

using Stealth::Engine::RTTBuffer;

template <typename T>
class RTTBufFixture {
protected:
    RTTBuffer buf{RTTBuffer::create<T>()};
};

using IntBufFixture = RTTBufFixture<int>;

STEST_F(IntBufFixture, CanConstruct) { }

STEST_F(IntBufFixture, TypeChecksWork) {
#ifdef S_DEBUG
    EXPECT_THROWS(buf.checkType<float>(), RTTBuffer::BadTypeCast);
#endif
}

int main(int argc, const char* argv[]) {
    return static_cast<int>(RUN_STESTS(argc, argv));
}
