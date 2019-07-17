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
    private:
        struct TypeInfo {
            const std::type_info& type;
            size_t size;
        } mTypeInfo;

        struct SizeInfo {
            size_t size, capacity;
        } mSizeInfo;

        std::unique_ptr<std::byte[]> mData{nullptr};

        RTTBuffer(TypeInfo&& info, size_t initialCapacity);

    public:
        class BadTypeCast : public std::exception { };

        template <typename T>
        static RTTBuffer create(size_t initialCapacity = 0) {
            return RTTBuffer{TypeInfo{typeid(T), sizeof(T)}, initialCapacity};
        }

        RTTBuffer() = delete;

        RTTBuffer(const RTTBuffer& other);
        RTTBuffer(RTTBuffer&& other);

        // TODO: Add Copy and Move assignment

        // Buffer operations
        void reserve(size_t newSize);

        template <typename T>
        T& push_back(T&& value) {
            return this->emplace_back<std::remove_cv_t<std::remove_reference_t<T>>>(std::forward<T&&>(value));
        }

        template <typename T, typename... Args>
        T& emplace_back(Args&&... args) {
            ++mSizeInfo.size;
            this->reserve(mSizeInfo.size);
            this->atUnchecked<T>(mSizeInfo.size - 1) = std::move(T{args...});
            return this->atUnchecked<T>(mSizeInfo.size - 1);
        }

        // Metadata
        constexpr size_t size() const { return mSizeInfo.size; }
        constexpr size_t capacity() const { return mSizeInfo.capacity; }
        constexpr size_t numBytes() const { return mSizeInfo.size * this->elementSize(); }
        constexpr size_t elementSize() const { return mTypeInfo.size; }
        constexpr const std::type_info& type() const { return mTypeInfo.type; }
        
        // Data accessors
        template <typename T>
        T& at(size_t index) {
            this->checkBounds(index);
            return this->atUnchecked<T>(index);
        }

        template <typename T>
        T& atUnchecked(size_t index) {
            return this->data<T>()[index];
        }

        template <typename T>
        const T& at(size_t index) const {
            this->checkBounds(index);
            return this->atUnchecked<T>(index);
        }

        template <typename T>
        const T& atUnchecked(size_t index) const {
            return this->data<T>()[index];
        }

        template <typename T>
        T* data() const {
            this->checkType<T>();
            return reinterpret_cast<T*>(mData.get());
        }

    private:
        template <typename T>
        void checkType() const {
#ifdef RTT_BUF_ENABLE_TYPE_CHECKS
            const auto& toType = typeid(T);
            if (toType != mTypeInfo.type) {
                LOG_ERROR() << "Could not cast " << mTypeInfo.type.name() << " (" << this->elementSize() << " bytes) to "
                    << toType.name() << " (" << sizeof(T) << " bytes)" << std::endl;
                throw BadTypeCast{};
            }
#endif
        }

        void checkBounds(size_t index) const;
    };
} // Stealth::Engine
