#include "rttbuffer.hpp"
#include <cstring>

namespace Stealth::Engine {
    RTTBuffer::RTTBuffer(TypeRef&& ref, size_t elementSize, size_t initialSize) : mType{ref}, mElementSize{elementSize} {
        this->resize(initialSize);
    }

    void RTTBuffer::resize(size_t newSize) {
        if (newSize <= mCapacity) {
            return;
        }

        // Copy old data to a larger buffer, then swap buffers.
        std::unique_ptr<std::byte[]> newData{new std::byte[newSize * this->elementSize()]};
        std::memcpy(newData.get(), mData.get(), mSize * this->elementSize());

        mData.swap(newData);
        mSize = newSize;
        mCapacity = newSize;
    }

    size_t RTTBuffer::size() const {
        return mSize;
    }

    size_t RTTBuffer::elementSize() const {
        return mElementSize;
    }

    const std::type_info& RTTBuffer::type() const {
        // return mType.get();
        return mType;
    }


} // Stealth::Engine
