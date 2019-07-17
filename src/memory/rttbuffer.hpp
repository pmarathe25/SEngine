#pragma once
#include <Stealth/SLog.hpp>
#include <typeinfo>
#include <functional>
#include <memory>

#ifdef S_DEBUG
#define RTT_BUF_ENABLE_TYPE_CHECKS
#endif

namespace Stealth::Engine {
    // A runtime type-checked buffer.
    class RTTBuffer {
    public:
        using TypeRef = std::reference_wrapper<const std::type_info>;

        class BadTypeCast : public std::exception { };

        template <typename T>
        static RTTBuffer create(size_t initialSize = 0) {
            return RTTBuffer{std::cref(typeid(T)), sizeof(T), initialSize};
        }

        RTTBuffer() = delete;
        RTTBuffer(TypeRef&& ref, size_t elementSize, size_t initialSize);

        template <typename T>
        void checkType() const {
#ifdef RTT_BUF_ENABLE_TYPE_CHECKS
            TypeRef toType = std::cref(typeid(T));
            if (toType.get() != mType.get()) {
                LOG_ERROR() << "Could not cast " << mType.get().name() << " (" << mElementSize << " bytes) to "
                    << toType.get().name() << " (" << sizeof(T) << " bytes)" << std::endl;
                throw BadTypeCast{};
            }
#endif
        }

        // TODO: Add Copy and Move constructors

        // TODO: Add Copy and Move assignment

        // Buffer operations
        void resize(size_t newSize);

        // TODO: Add push_back and emplace_back

        // Metadata
        size_t size() const;
        size_t elementSize() const;
        const std::type_info& type() const;

        // Data accessors
        template <typename T>
        T& at(size_t index) const {
            if (index >= mSize) {
                throw std::out_of_range{std::to_string(index) + " is out of range of this buffer, which has size: " + std::to_string(mSize)};
            }
            return this->atUnchecked<T>(index);
        }

        template <typename T>
        T& atUnchecked(size_t index) const {
            return this->data<T>()[index];
        }

        template <typename T>
        T* data() const {
            this->checkType<T>();
            return reinterpret_cast<T*>(mData.get());
        }
    private:
        TypeRef mType;
        std::unique_ptr<std::byte[]> mData{nullptr};
        size_t mSize, mCapacity, mElementSize;
    };
} // Stealth::Engine
