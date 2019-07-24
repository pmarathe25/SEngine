#ifndef ARCHETYPE_HPP
#define ARCHETYPE_HPP
#include "meta/functions.hpp"
#include <tuple>
#include <vector>

namespace Stealth::Engine {
    template <typename... ComponentTypes>
    class Archetype {
    public:
        template <typename ComponentType>
        using StorageType = std::vector<ComponentType>;
    protected:
        std::tuple<StorageType<ComponentTypes>...> mComponents;
        size_t mSize{0}; // The number of entities in this Archetype.
    public:

        // When an Archetype is constructed, it will check whether it contains duplicate component types
        constexpr Archetype() {
            static_assert(packIsUnique<ComponentTypes...>(), "Archetype cannot contain duplicate component types");
        }

        constexpr size_t size() const {
            return mSize;
        }

        template <typename T>
        static constexpr bool contains() {
            return packContains<T, ComponentTypes...>();
        }

        // Adds the provided components to mComponents and returns the index of the newly added components.
        template <typename... Args>
        size_t addComponents(Args&&... components) {
            static_assert(packsAreEquivalent(std::tuple<removeCVRef<Args>...>{}, std::tuple<ComponentTypes...>{}), "Component types do not match the types of this Archetype");
            // Add each component to the appropriate vector.
            (std::get<StorageType<removeCVRef<Args>>>(mComponents).emplace_back(std::forward<Args&&>(components)), ...);
            return mSize++;
        }
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
