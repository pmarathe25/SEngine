#include "archetype.hpp"

namespace Stealth::Engine {
    ArchetypeGroup::ArchetypeGroup(ComponentMap components) : mComponents{std::move(components)} { }
} // Stealth::Engine
