#pragma once
#include <typeinfo>
#include <functional>

namespace Stealth::Engine {
    // A runtime type-checked buffer.
    class RTTBuffer {
    public:
        template <typename T>
        static RTTBuffer create() {
            return RTTBuffer{std::cref(typeid(T))};
        }

        using TypeRef = std::reference_wrapper<const std::type_info>;
        RTTBuffer() = delete;
        RTTBuffer(TypeRef&& ref);

        // TODO: Add Copy and Move constructors

        // TODO: Add Copy and Move assignment

        // TODO: Add type and bounds checked accessors

        // TODO: Add type-checked data().

    private:
        TypeRef mType;
        std::byte* mData;
    };
} // Stealth::Engine
