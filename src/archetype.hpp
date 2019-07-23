#ifndef ARCHETYPE_HPP
#define ARCHETYPE_HPP
#include "meta/functions.hpp"
#include <tuple>
#include <vector>

namespace Stealth::Engine {
    template <typename... ComponentTypes>
    class ArchetypeGroup {
    private:
        std::tuple<std::vector<ComponentTypes>...> mComponents;
    public:
        constexpr ArchetypeGroup() {
            static_assert(packIsUnique<ComponentTypes...>(), "Cannot create ArchetypeGroup with duplicate component types");
        }
    };

    template <typename... ComponentTypes>
    struct Archetype {
        using GroupType = ArchetypeGroup<ComponentTypes...>;
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
} // std

#endif // ARCHETYPE_HPP
