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
    // TODO: Use a TypedView so that every single function doesn't need to be provided the type.
    class RTTBuffer {
    public:
        class BadTypeCast : public std::exception { };

        template <typename T>
        static RTTBuffer create(size_t initialCapacity = 0) {
            return RTTBuffer{TypeInfo{typeid(T), sizeof(T)}, initialCapacity};
        }
        
    private:
        struct TypeInfo {
            using TypeRef = std::reference_wrapper<const std::type_info>;
            TypeRef type;
            size_t size{0};
        } mTypeInfo;

        struct SizeInfo {
            size_t size{0}, capacity{0};
        } mSizeInfo;

        std::unique_ptr<std::byte[]> mData{nullptr};

        RTTBuffer(TypeInfo&& info, size_t initialCapacity);
        RTTBuffer() = delete;

    public:
        RTTBuffer(const RTTBuffer& other);
        RTTBuffer(RTTBuffer&& other);
        RTTBuffer& operator=(const RTTBuffer& other);
        RTTBuffer& operator=(RTTBuffer&& other);

        // Buffer operations
        void reserve(size_t newSize);

        template <typename T>
        T& pushBack(T&& value) {
            return this->emplaceBack<std::remove_cv_t<std::remove_reference_t<T>>>(std::forward<T&&>(value));
        }

        template <typename T, typename... Args>
        T& emplaceBack(Args&&... args) {
            // Binary exponential backoff, reserving at least 2 spots.
            if (mSizeInfo.size == mSizeInfo.capacity) {
                this->reserve((mSizeInfo.capacity ? mSizeInfo.capacity : 1) * 2);
            }
            ++mSizeInfo.size;
            this->atUnchecked<T>(mSizeInfo.size - 1) = std::move(T{args...});
            return this->atUnchecked<T>(mSizeInfo.size - 1);
        }

        // Metadata
        constexpr size_t size() const { return mSizeInfo.size; }
        constexpr size_t capacity() const { return mSizeInfo.capacity; }
        constexpr size_t numBytes() const { return mSizeInfo.size * this->elementSize(); }
        constexpr size_t elementSize() const { return mTypeInfo.size; }
        constexpr const std::type_info& type() const { return mTypeInfo.type.get(); }

        // Data accessors
        template <typename T>
        T& at(size_t index) {
            this->checkBounds(index);
            return this->atUnchecked<T>(index);
        }

        template <typename T>
        const T& at(size_t index) const {
            this->checkBounds(index);
            return this->atUnchecked<T>(index);
        }

        template <typename T>
        T& atUnchecked(size_t index) {
            return this->data<T>()[index];
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
            if (toType != mTypeInfo.type.get()) {
                LOG_ERROR() << "Could not cast " << mTypeInfo.type.get().name() << " (" << this->elementSize() << " bytes) to "
                    << toType.name() << " (" << sizeof(T) << " bytes)" << std::endl;
                throw BadTypeCast{};
            }
#endif
        }

        void checkBounds(size_t index) const;
    };
} // Stealth::Engine
