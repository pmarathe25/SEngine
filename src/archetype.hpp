#ifndef ARCHETYPE_HPP
#define ARCHETYPE_HPP
#include "meta/functions.hpp"
#include <tuple>
#include <vector>

namespace Stealth::Engine {
    template <typename... ComponentTypes>
    struct Archetype;

    template <typename... ComponentTypes>
    class ArchetypeGroup {
    private:
        std::tuple<std::vector<ComponentTypes>...> mComponents;
    public:
        // When an ArchetypeGroup is constructed, it will check whether it contains duplicate component types
        // using Archetype's static_asserts.
        constexpr ArchetypeGroup(Archetype<ComponentTypes...> = {}) { }
    };

    template <typename... ComponentTypes>
    struct Archetype {
        static_assert(packIsUnique<ComponentTypes...>(), "Archetype cannot contain duplicate component types");

        using GroupType = ArchetypeGroup<ComponentTypes...>;

        template <typename T>
        static constexpr bool contains = packContains<T, ComponentTypes...>();
    };
} // Stealth::Engine

namespace std {
    // This overload allows us to treat a pack of packs (i.e. pack of archetypes) as a single-layer deep pack.
    // i.e. a pack of archetypes can be checked for duplicates even when type orders might be
    // different between archetypes.
    template <typename... Args1, typename... Args2>
    struct is_same<Stealth::Engine::Archetype<Args1...>, Stealth::Engine::Archetype<Args2...>> {
        static constexpr bool value = Stealth::Engine::packsAreEquivalent(std::tuple<Args1...>{}, std::tuple<Args2...>{});
    };

    // To disambiguate from std::is_same<_Tp, _Tp> specialization.
    template <typename... Args1>
    struct is_same<Stealth::Engine::Archetype<Args1...>, Stealth::Engine::Archetype<Args1...>> {
        static constexpr bool value = true;
    };
} // std

#endif // ARCHETYPE_HPP
