#ifndef META_TUPLES_HPP
#define META_TUPLES_HPP
#include "meta/packs.hpp"
#include <tuple>

// Forward declarations
namespace Stealth::Engine {
    // Selects values of the specified type from the input tuple.
    template <typename... Select, typename... From>
    constexpr std::tuple<Select...> tupleSelect(std::tuple<From...> fromPack);

} // Stealth::Engine

namespace Stealth::Engine {
    template <typename... Select, typename... From>
    constexpr std::tuple<Select...> tupleSelect(std::tuple<From...> fromPack) {
        static_assert(packIsSubset(ParameterPack<Select...>{}, ParameterPack<From...>{}), "Types to select must be a subset of the input tuple");
        // Move all elements from the old tuple to the new one.
        return std::tuple<Select...>{std::move(std::get<Select>(fromPack))...};
    }
} // Stealth::Engine

#endif // META_TUPLES_HPP
