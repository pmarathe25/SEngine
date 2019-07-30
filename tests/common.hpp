#ifndef COMMON_HPP
#define COMMON_HPP
#include "archetype.hpp"
#include <Stealth/STest.hpp>


namespace common {
    using Stealth::Engine::Archetype;
    using IFArchetype = Archetype<int, float>;

    // Test Archetype protected things by deriving from it.
    class ArchetypeInternalFixture : public IFArchetype {
    public:
        ArchetypeInternalFixture() : IFArchetype{} {
            EXPECT_EQ(intStorage.size(), 0);
            EXPECT_EQ(floatStorage.size(), 0);
        }
    protected:
        const IFArchetype::StorageType<int>& intStorage{std::get<IFArchetype::StorageType<int>>(mComponents)};
        const IFArchetype::StorageType<float>& floatStorage{std::get<IFArchetype::StorageType<float>>(mComponents)};
    };
} // common

#endif // COMMON_HPP
