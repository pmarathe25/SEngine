#ifndef WORLD_HPP
#define WORLD_HPP
#include "archetype.hpp"
#include <tuple>

namespace Stealth::Engine {
    using Entity = size_t;

    // The world manages all entity creation, destruction and modification (adding/removing components)
    template <typename... Archetypes>
    class World {
    private:
        std::tuple<ArchetypeGroup<typename Archetypes::GroupType>...> mGroups{};
    public:
        World() {
            static_assert(packIsUnique<Archetypes...>(), "World cannot contain duplicate archetypes");
        }
    };
} // Stealth::Engine

#endif // WORLD_HPP
