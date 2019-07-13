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
        ArchetypeGroup() {
            static_assert(packIsUnique<ComponentTypes...>(), "Cannot create ArchetypeGroup with duplicate component types");
        }
    };
} // Stealth::Engine

#endif // ARCHETYPE_HPP
