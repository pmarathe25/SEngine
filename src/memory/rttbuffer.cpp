#include "rttbuffer.hpp"
#include <cstring>

namespace Stealth::Engine {
    RTTBuffer::RTTBuffer(RTTBuffer::TypeInfo&& typeInfo, size_t initialCapacity) : mTypeInfo{typeInfo} {
        this->reserve(initialCapacity);
    }

    RTTBuffer::RTTBuffer(const RTTBuffer& other) : mTypeInfo{other.mTypeInfo}, mSizeInfo{other.mSizeInfo}, mData{new std::byte[other.capacity() * other.elementSize()]} {
        std::memcpy(mData.get(), other.mData.get(), other.capacity() * other.elementSize());
    }

    RTTBuffer::RTTBuffer(RTTBuffer&& other) : mTypeInfo{other.mTypeInfo}, mSizeInfo{other.mSizeInfo}, mData{other.mData.release()} {
        other.mSizeInfo.capacity = 0;
        other.mSizeInfo.size = 0;
    }

    RTTBuffer& RTTBuffer::operator=(const RTTBuffer& other) {
        mTypeInfo = other.mTypeInfo;
        mSizeInfo = other.mSizeInfo;
        mData.reset(new std::byte[other.capacity() * other.elementSize()]);
        std::memcpy(mData.get(), other.mData.get(), other.capacity() * other.elementSize());
        return (*this);
    }

    RTTBuffer& RTTBuffer::operator=(RTTBuffer&& other) {
        mTypeInfo = other.mTypeInfo;
        mSizeInfo = other.mSizeInfo;
        mData.reset(other.mData.release());

        other.mSizeInfo.capacity = 0;
        other.mSizeInfo.size = 0;
        return (*this);
    }

    void RTTBuffer::reserve(size_t newSize) {
        LOG_DEBUG() << "Requested reservation of " << newSize << " elements" << std::endl;
        if (newSize <= mSizeInfo.capacity) {
            return;
        }

        // Copy old data to a larger buffer, then swap buffers.
        std::unique_ptr<std::byte[]> newData{new std::byte[newSize * this->elementSize()]};
        std::memcpy(newData.get(), mData.get(), this->numBytes());

        mData.swap(newData);
        mSizeInfo.capacity = newSize;
    }

    void RTTBuffer::checkBounds(size_t index) const {
        if (index >= mSizeInfo.size) {
            throw std::out_of_range{std::to_string(index) + " is out of range of this buffer, which has size: " + std::to_string(mSizeInfo.size)};
        }
    }
} // Stealth::Engine
