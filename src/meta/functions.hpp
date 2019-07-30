#ifndef VARIADIC_HELPERS_HPP
#define VARIADIC_HELPERS_HPP
#include <type_traits>
#include <tuple>

// All comparisons rely on std::is_same, and will therefore respect any specializations of that metafunction.
namespace Stealth::Engine {
    template <typename T>
    using removeCVRef = std::remove_cv_t<std::remove_reference_t<T>>;

    // Determines whether a parameter pack, Args, contains the specified type, Elem.
    template <typename Elem, typename... Args>
    constexpr bool packContains() noexcept;

    // Determines the index at which a parameter pack, Args, contains the specified type, Elem.
    template <typename Elem, typename... Args>
    constexpr int packIndex() noexcept;

    // Determines whether a parameter pack contains unique types.
    template <typename... Args>
    constexpr bool packIsUnique() noexcept;

    // TODO: Pass tuple as template argument.
    // Determines whether the left-hand side pack is a subset of the right-hand side pack.
    template <typename... Args1, typename... Args2>
    constexpr bool packIsSubset(const std::tuple<Args2...>& rhs) noexcept;

    // Determines whether two packs contain the same types, regardless of order.
    template <typename... Args1, typename... Args2>
    constexpr bool packsAreEquivalent(const std::tuple<Args2...>& rhs) noexcept;

    template <typename... To, typename... From>
    constexpr std::tuple<To...> reorderTuple(std::tuple<From...> fromPack);
} // Stealth::Engine

namespace Stealth::Engine {
    template <typename Elem, typename... Args>
    constexpr bool packContains() noexcept {
        return (std::is_same_v<Elem, Args> || ...);
    }


    template <typename Elem, int Index, typename First, typename... Args>
    constexpr int packIndexImpl() noexcept {
        if constexpr (std::is_same_v<Elem, First>) {
            return Index;
        } else {
            // sizeof...(Args) will always be at least 1. The function will return before it reaches 0.
            return packIndexImpl<Elem, Index + 1, Args...>();
        }
    }

    template <typename Elem, typename... Args>
    constexpr int packIndex() noexcept {
        static_assert(packContains<Elem, Args...>(), "Cannot find index of type, because pack does not contain this type");
        return packIndexImpl<Elem, 0, Args...>();
    }


    template <typename T1, typename... Args>
    constexpr bool packIsUniqueImpl() noexcept {
        return (!packContains<T1, Args...>() && packIsUnique<Args...>());
    }

    template <typename... Args>
    constexpr bool packIsUnique() noexcept {
        if constexpr (sizeof...(Args) <= 1) {
            return true;
        } else {
            return packIsUniqueImpl<Args...>();
        }
    }


    // Loops over LHS and checks whether each type is present in the RHS
    template <typename T1, typename... Args1, typename... Args2>
    constexpr bool packIsSubsetImpl(const std::tuple<Args2...>& rhs) noexcept {
        return packContains<T1, Args2...>() && packIsSubset<Args1...>(rhs);
    }

    template <typename... Args1, typename... Args2>
    constexpr bool packIsSubset(const std::tuple<Args2...>& rhs) noexcept {
        if constexpr (sizeof...(Args1) == 0) {
            return true;
        } else {
            return packIsSubsetImpl<Args1...>(rhs);
        }
    }


    template <typename... Args1, typename... Args2>
    constexpr bool packsAreEquivalent(const std::tuple<Args2...>& rhs) noexcept {
        static_assert(packIsUnique<Args1...>() && packIsUnique<Args2...>(), "Cannot compare non-unique packs for equivalency!");
        if constexpr (sizeof...(Args1) != sizeof...(Args2)) {
            return false;
        } else if constexpr (sizeof...(Args1) <= 1 || sizeof...(Args2) <= 1) {
            return true;
        } else {
            return packIsSubset<Args1...>(rhs);
        }
    }

    template <typename... To, typename... From>
    constexpr std::tuple<To...> reorderTuple(std::tuple<From...> fromPack) {
        static_assert(packsAreEquivalent<To...>(fromPack), "New pack must contain the same types as the old pack");
        // Move all elements from the old tuple to the new one.
        return std::tuple<To...>{std::move(std::get<To>(fromPack))...};
    }

} // Stealth::Engine

#endif // VARIADIC_HELPERS_HPP
