#ifndef META_PACKS_HPP
#define META_PACKS_HPP
#include <type_traits>

// All comparisons rely on std::is_same, and will therefore respect any specializations of that metafunction.
namespace Stealth::Engine {
    template <typename... Parameters>
    struct ParameterPack {
    public:
        // Returns true if this pack contains any of the specified types.
        template <typename... Args>
        static constexpr bool contains() noexcept {
            if constexpr (sizeof...(Args) < 1) {
                return true;
            } else {
                return containsImpl<Args...>();
            }
        }

        // Returns the index of the specified type in this pack.
        template <typename Elem>
        static constexpr std::size_t index() noexcept {
            static_assert(contains<Elem>(), "Cannot find index of type, because pack does not contain this type");
            return indexImpl<Elem, 0, Parameters...>();
        }

        static constexpr bool isUnique() noexcept {
            if constexpr (sizeof...(Parameters) <= 1) {
                return true;
            } else {
                return isUniqueImpl<Parameters...>();
            }
        }

        template <typename... Args>
        static constexpr bool equivalent() noexcept {
            if constexpr (sizeof...(Parameters) != sizeof...(Args)) {
                return false;
            } else {
                return contains<Args...>();
            }
        }

    protected:
        template <typename Elem, typename... Rest>
        static constexpr bool containsImpl() noexcept {
            return (std::is_same_v<Elem, Parameters> || ...) && contains<Rest...>();
        }

        template <typename Elem, int Index, typename First, typename... Rest>
        static constexpr std::size_t indexImpl() noexcept {
            if constexpr (std::is_same_v<Elem, First>) {
                return Index;
            } else {
                // sizeof...(Rest) will always be at least 1. The function will return before it reaches 0.
                // This is guaranteed by the contains() check.
                return indexImpl<Elem, Index + 1, Rest...>();
            }
        }

        template <typename First, typename... Rest>
        static constexpr bool isUniqueImpl() noexcept {
            using RestPack = ParameterPack<Rest...>;
            return (!RestPack::template contains<First>() && RestPack::isUnique());
        }
    };

    template <typename T>
    using removeCVRef = std::remove_cv_t<std::remove_reference_t<T>>;

} // Stealth::Engine

#endif // META_PACKS_HPP
