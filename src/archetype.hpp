#ifndef ARCHETYPE_HPP
#define ARCHETYPE_HPP
#include "memory/bidirectionalMap.hpp"
#include "meta/helpers.hpp"
#include "meta/packs.hpp"
#include "entity.hpp"
#include <cstddef> // For std::size_t
#include <vector>

namespace Stealth::Engine {
    template <typename... ComponentTypes>
    class Archetype {
    public:
        template <typename T>
        using StorageType = std::vector<T>;

        using ComponentPack = Pack<ComponentTypes...>;
        using ComponentStoragePack = Pack<StorageType<ComponentTypes>...>;

        constexpr Archetype() {
            static_assert(ComponentPack::isUnique(), "Archetype cannot contain duplicate component types");
        }

        // Adds the provided components to mComponents and returns the index of the newly added components.
        template <typename... Args>
        std::size_t addComponents(EntityID entity, Args&&... components) {
            static_assert(ComponentPack::template equivalent<removeCVRef<Args>...>(), "Component types do not match the types of this Archetype");
            // Add each component to the appropriate vector.
            (this->storage<removeCVRef<Args>>().emplace_back(std::forward<Args&&>(components)), ...);

            std::size_t index = mSize++;
            mEntityMap.set(entity, index);
            return index;
        }

        // constexpr ComponentPack remove(EntityID index) {
        //     // TODO: Fill this out
        // }
    protected:
        template <typename ComponentType>
        constexpr StorageType<ComponentType>& storage() {
            static_assert(ComponentPack::template contains<ComponentType>(), "Cannot retrieve component type - this type is not present in the archetype");
            return mComponents.template at<StorageType<ComponentType>>();
        }

        BidirectionalMap<EntityID, std::size_t> mEntityMap; // Maps entities to their component indexes.
        ComponentStoragePack mComponents;
        std::size_t mSize{0}; // The number of entities in this Archetype.
    };
} // Stealth::Engine

namespace std {
    // This overload allows us to treat a pack of packs (i.e. pack of archetypes) as a single-layer deep pack.
    // i.e. a pack of archetypes can be checked for duplicates even when type orders might be
    // different between archetypes.
    template <typename... Args1, typename... Args2>
    struct is_same<Stealth::Engine::Archetype<Args1...>, Stealth::Engine::Archetype<Args2...>> {
        using Pack1 = Stealth::Engine::Pack<Args1...>;
        static constexpr bool value = Pack1::template equivalent<Args2...>();
    };

    // To disambiguate from std::is_same<_Tp, _Tp> specialization.
    template <typename... Args1>
    struct is_same<Stealth::Engine::Archetype<Args1...>, Stealth::Engine::Archetype<Args1...>> {
        static constexpr bool value = true;
    };
} // std

#endif // ARCHETYPE_HPP
