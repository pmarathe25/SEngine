#pragma once
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include "rttbuffer.hpp"

namespace Stealth::Engine {
    // An ArchetypeGroup contains all entities of a given archetype.
    // Essentially, each group contains one RTTBuffer per component type.
    class ArchetypeGroup {
    public:
        using ComponentMap = std::unordered_map<std::type_index, RTTBuffer>;

        // New component types cannot be added after construction - a new ArchetypeGroup must be created instead.
        template <typename... Args>
        static ArchetypeGroup create() {
            ComponentMap components{sizeof...(Args)};
            (components.emplace(typeid(Args), RTTBuffer::create<Args>()), ...);
            LOG_DEBUG() << "Constructing ArchetypeGroup with types: ";
            (LOG_RAW(1) << ... << (typeid(Args).name() + std::string{", "})) << std::endl;
            return ArchetypeGroup{std::move(components)};
        }
    private:
        ComponentMap mComponents{};
        ArchetypeGroup(ComponentMap components);
        ArchetypeGroup() = delete;
    public:
        // When adding components, all components must be added together.
        // This ensures that entities remain synchronized.
        // Returns the index of the newly added entity.
        template <typename... Args>
        size_t addEntity(Args&&... args) {
            // TODO: Add checks here to ensure Args match the types in this group.
            (mComponents[typeid(args)].emplaceBack(std::forward<Args&&>(args)), ...);
        }

        // TODO: Revise API as needed.
        const ComponentMap& getComponents() const {
            return mComponents;
        }

    };
} // Stealth::Engine
