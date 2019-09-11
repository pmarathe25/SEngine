#ifndef META_PACKS_HPP
#define META_PACKS_HPP
#include <type_traits>
#include <tuple>
#include <cstddef>

// TODO: Implement Pack::visit

// All comparisons rely on std::is_same, and will therefore respect any specializations of that metafunction.
namespace Stealth::Engine {
    template <typename... Types>
    struct Pack : public std::tuple<Types...> {
    public:
        // Use tuple's constructors
        using Base = std::tuple<Types...>;
        using Base::Base;

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
        static constexpr size_t index() noexcept {
            static_assert(contains<Elem>(), "Cannot find index of type, because pack does not contain this type");
            return indexImpl<Elem, 0, Types...>();
        }

        static constexpr bool isUnique() noexcept {
            if constexpr (sizeof...(Types) <= 1) {
                return true;
            } else {
                return isUniqueImpl<Types...>();
            }
        }

        template <typename... Args>
        static constexpr bool equivalent() noexcept {
            if constexpr (sizeof...(Types) != sizeof...(Args)) {
                return false;
            } else {
                return contains<Args...>();
            }
        }

        template <std::size_t Index>
        auto& at() noexcept {
            return std::get<Index>(*this);
        }

        template <std::size_t Index>
        const auto& at() const noexcept {
            return std::get<Index>(*this);
        }

        template <typename Type>
        Type& at() noexcept {
            return std::get<Type>(*this);
        }

        template <typename Type>
        const Type& at() const noexcept {
            return std::get<Type>(*this);
        }

    protected:
        template <typename Elem, typename... Rest>
        static constexpr bool containsImpl() noexcept {
            return (std::is_same_v<Elem, Types> || ...) && contains<Rest...>();
        }

        template <typename Elem, int Index, typename First, typename... Rest>
        static constexpr size_t indexImpl() noexcept {
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
            using RestPack = Pack<Rest...>;
            return (!RestPack::template contains<First>() && RestPack::isUnique());
        }
    };

} // Stealth::Engine

namespace std {
    template<typename... Types>
    struct tuple_size<Stealth::Engine::Pack<Types...>> : std::tuple_size<std::tuple<Types...>> {};

    template<std::size_t N, typename... Types>
    struct tuple_element<N, Stealth::Engine::Pack<Types...>> : std::tuple_element<N, std::tuple<Types...>> {};
} // std

#endif // META_PACKS_HPP
