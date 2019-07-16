#pragma once
#include <Stealth/SLog.hpp>
#include <typeinfo>
#include <functional>

namespace Stealth::Engine {
    // A runtime type-checked buffer.
    class RTTBuffer {
    public:
        using TypeRef = std::reference_wrapper<const std::type_info>;

        class BadTypeCast : public std::exception { };

        template <typename T>
        static RTTBuffer create() {
            return RTTBuffer{std::cref(typeid(T)), sizeof(T)};
        }

        RTTBuffer() = delete;
        RTTBuffer(TypeRef&& ref, size_t elementSize);

        template <typename T>
        void checkType() const {
#ifdef S_DEBUG
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

        // TODO: Add type and bounds checked accessors

        template <typename T>
        T* data() const {
            this->checkType<T>();
            return static_cast<T*>(mData);
        }

    private:
        TypeRef mType;
        std::byte* mData;
        size_t mSize, mCapacity, mElementSize;
    };
} // Stealth::Engine
