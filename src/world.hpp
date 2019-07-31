#ifndef WORLD_HPP
#define WORLD_HPP
#include "archetype.hpp"
#include "meta/packs.hpp"
#include <tuple>

namespace Stealth::Engine {
    using Entity = size_t;

    // The world manages all entity creation, destruction and modification (adding/removing components)
    template <typename... Archetypes>
    class World {
    public:
        using ArchetypePack = ParameterPack<Archetypes...>;

        World() {
            static_assert(ArchetypePack::isUnique(), "World cannot contain duplicate archetypes");
        }

        template <typename... ComponentTypes>
        Entity createEntity(ComponentTypes&&... components) {
            using EntityArchetype = Archetype<removeCVRef<ComponentTypes>...>;
            static_assert(ArchetypePack::template contains<EntityArchetype>(), "This archetype has not been registered");
            // Find Archetype in mArchetypes
            static constexpr int Index = ArchetypePack::template index<EntityArchetype>();
            auto& archetype = std::get<Index>(mArchetypes);
            // Add entity to appropriate archetype.
            archetype.addComponents(std::forward<ComponentTypes&&>(components)...);
            return mLastEntity++;
        }
    protected:
        std::tuple<Archetypes...> mArchetypes{};
        Entity mLastEntity{0};
    };
} // Stealth::Engine

#endif // WORLD_HPP
