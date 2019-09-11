#ifndef WORLD_HPP
#define WORLD_HPP
#include "entity.hpp"
#include "archetype.hpp"
#include "meta/helpers.hpp"
#include "meta/packs.hpp"

namespace Stealth::Engine {
    // The world manages all entity creation, destruction and modification (adding/removing components)
    template <typename... Archetypes>
    class World {
    public:
        using ArchetypePack = Pack<Archetypes...>;

        World() {
            static_assert(ArchetypePack::isUnique(), "World cannot contain duplicate archetypes");
        }

        template <typename... ComponentTypes>
        Entity createEntity(ComponentTypes&&... components) {
            using EntityArchetype = Archetype<removeCVRef<ComponentTypes>...>;
            static_assert(ArchetypePack::template contains<EntityArchetype>(), "This archetype has not been registered");
            // Find Archetype in mArchetypes
            static constexpr int Index = ArchetypePack::template index<EntityArchetype>();
            auto& archetype = mArchetypes.template at<Index>();
            // Add entity to appropriate archetype.
            archetype.addComponents(std::forward<ComponentTypes&&>(components)...);
            return mNextEntity++;
        }

        // TODO: Implement destroyEntity using Pack::visit.

    protected:
        ArchetypePack mArchetypes{};
        Entity mNextEntity{0};
    };
} // Stealth::Engine

#endif // WORLD_HPP
