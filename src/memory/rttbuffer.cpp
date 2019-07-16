#include "rttbuffer.hpp"

namespace Stealth::Engine {
    RTTBuffer::RTTBuffer(TypeRef&& ref, size_t elementSize) : mType{ref}, mElementSize{elementSize} { }
} // Stealth::Engine
