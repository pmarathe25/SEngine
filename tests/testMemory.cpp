#include <Stealth/STest.hpp>
#include "rttbuffer.hpp"

using Stealth::Engine::RTTBuffer;

template <typename T>
class RTTBufferFixture {
protected:
    RTTBufferFixture() : buf{RTTBuffer::create<T>()} { }
    RTTBuffer buf;
};

using IntBufFixture = RTTBufferFixture<int>;

STEST_F(IntBufFixture, CanConstruct) { }

int main(int argc, const char* argv[]) {
    return static_cast<int>(RUN_STESTS(argc, argv));
}
