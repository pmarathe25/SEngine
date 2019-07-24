#ifndef WORLD_HPP
#define WORLD_HPP
#include "archetype.hpp"
#include <tuple>

namespace Stealth::Engine {
    using Entity = size_t;

    // The world manages all entity creation, destruction and modification (adding/removing components)
    template <typename... Archetypes>
    class World {
    protected:
        std::tuple<Archetypes...> mArchetypes{};
        size_t lastID{0};
    public:
        World() {
            static_assert(packIsUnique<Archetypes...>(), "World cannot contain duplicate archetypes");
        }

        template <typename... ComponentTypes>
        Entity createEntity(ComponentTypes&&... components) {
            using EntityArchetype = Archetype<removeCVRef<ComponentTypes>...>;
            static_assert(packContains<EntityArchetype, Archetypes...>(), "This archetype has not been registered");
            // Find Archetype in mArchetypes
            static constexpr int Index = packIndex<EntityArchetype, Archetypes...>();
            auto& archetype = std::get<Index>(mArchetypes);
            // Add entity to appropriate archetype.
            archetype.addComponents(std::forward<ComponentTypes&&>(components)...);
            return lastID++;
        }
    };
} // Stealth::Engine

#endif // WORLD_HPP
