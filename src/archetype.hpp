#ifndef ARCHETYPE_HPP
#define ARCHETYPE_HPP
#include "meta/packs.hpp"
#include "meta/tuples.hpp"
#include <tuple>
#include <vector>

namespace Stealth::Engine {
    template <typename... ComponentTypes>
    class Archetype {
    public:
        template <typename T>
        using StorageType = std::vector<T>;

        using ComponentPack = ParameterPack<ComponentTypes...>;

        // When an Archetype is constructed, it will check whether it contains duplicate component types
        constexpr Archetype() {
            static_assert(ComponentPack::isUnique(), "Archetype cannot contain duplicate component types");
        }

        constexpr size_t size() const {
            return mSize;
        }

        // Adds the provided components to mComponents and returns the index of the newly added components.
        template <typename... Args>
        size_t addComponents(Args&&... components) {
            static_assert(ComponentPack::template equivalent<removeCVRef<Args>...>(), "Component types do not match the types of this Archetype");
            // Add each component to the appropriate vector.
            (std::get<StorageType<removeCVRef<Args>>>(mComponents).emplace_back(std::forward<Args&&>(components)), ...);
            return mSize++;
        }

        template <typename ComponentType>
        constexpr const StorageType<ComponentType>& storage() const {
            static_assert(ComponentPack::template contains<ComponentType>(), "Cannot retrieve component type - this type is not present in the archetype");
            return std::get<StorageType<ComponentType>>(mComponents);
        }

        // Returns a tuple of references to components in this archetype.
        // Types and ordering can be specified, but defaults to the types and ordering of the Archetype.
        // This function can also be used to select a subset of the component types.
        template <typename... SelectedComponents>
        constexpr std::tuple<const SelectedComponents&...> at(size_t index) const {
            return tupleSelect<const SelectedComponents&...>(std::tuple<const ComponentTypes&...>{this->storage<ComponentTypes>().at(index)...});
        }

        constexpr std::tuple<const ComponentTypes&...> at(size_t index) const {
            return this->at<ComponentTypes...>(index);
        }
    protected:
        std::tuple<StorageType<ComponentTypes>...> mComponents;
        size_t mSize{0}; // The number of entities in this Archetype.
    };
} // Stealth::Engine

namespace std {
    // This overload allows us to treat a pack of packs (i.e. pack of archetypes) as a single-layer deep pack.
    // i.e. a pack of archetypes can be checked for duplicates even when type orders might be
    // different between archetypes.
    template <typename... Args1, typename... Args2>
    struct is_same<Stealth::Engine::Archetype<Args1...>, Stealth::Engine::Archetype<Args2...>> {
        using Pack1 = Stealth::Engine::ParameterPack<Args1...>;
        static constexpr bool value = Pack1::template equivalent<Args2...>();
    };

    // To disambiguate from std::is_same<_Tp, _Tp> specialization.
    template <typename... Args1>
    struct is_same<Stealth::Engine::Archetype<Args1...>, Stealth::Engine::Archetype<Args1...>> {
        static constexpr bool value = true;
    };
} // std

#endif // ARCHETYPE_HPP
