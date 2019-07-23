#ifndef VARIADIC_HELPERS_HPP
#define VARIADIC_HELPERS_HPP
#include <type_traits>

// TODO: Need a way to find a type in a tuple.

// Forward Declarations
namespace Stealth::Engine {
    // Determines whether a parameter pack, Args, contains the specified type, Elem.
    template <typename Elem, typename... Args>
    constexpr bool packContains() noexcept;

    // Determines whether a parameter pack contains unique types.
    template <typename... Args>
    constexpr bool packIsUnique() noexcept;

    // Determines whether the left-hand side pack is a subset of the right-hand side pack.
    template <typename... Args1, typename... Args2>
    constexpr bool packIsSubset(const std::tuple<Args1...>& lhs, const std::tuple<Args2...>& rhs) noexcept;

    // Determines whether two packs contain the same types, regardless of order.
    template <typename... Args1, typename... Args2>
    constexpr bool packsAreEquivalent(const std::tuple<Args1...>& lhs, const std::tuple<Args2...>& rhs) noexcept;
} // Stealth::Engine

namespace Stealth::Engine {
    template <typename Elem, typename... Args>
    constexpr bool packContains() noexcept {
        return (std::is_same_v<Elem, Args> || ...);
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
    constexpr bool packIsSubsetImpl(const std::tuple<T1, Args1...>& lhs, const std::tuple<Args2...>& rhs) noexcept {
        return packContains<T1, Args2...>() && packIsSubset(std::tuple<Args1...>{}, rhs);
    }

    template <typename... Args1, typename... Args2>
    constexpr bool packIsSubset(const std::tuple<Args1...>& lhs, const std::tuple<Args2...>& rhs) noexcept {
        if constexpr (sizeof...(Args1) == 0) {
            return true;
        } else {
            return packIsSubsetImpl(lhs, rhs);
        }
    }


    template <typename... Args1, typename... Args2>
    constexpr bool packsAreEquivalent(const std::tuple<Args1...>& lhs, const std::tuple<Args2...>& rhs) noexcept {
        static_assert(packIsUnique<Args1...>() && packIsUnique<Args2...>(), "Cannot compare non-unique packs for equivalency!");
        if constexpr (sizeof...(Args1) != sizeof...(Args2)) {
            return false;
        } else if constexpr (sizeof...(Args1) <= 1 || sizeof...(Args2) <= 1) {
            return true;
        } else {
            return packIsSubset(lhs, rhs);
        }
    }

} // Stealth::Engine

#endif // VARIADIC_HELPERS_HPP
