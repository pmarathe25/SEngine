#ifndef COMMON_HPP
#define COMMON_HPP
#include "archetype.hpp"
#include <STest.hpp>


namespace common {
    using Stealth::Engine::Archetype;
    using IFArchetype = Archetype<int32_t, float>;

    // Test Archetype protected things by deriving from it.
    class ArchetypeInternalFixture : public IFArchetype {
    public:
        ArchetypeInternalFixture() : IFArchetype{} {
            EXPECT_EQ(intStorage.size(), 0);
            EXPECT_EQ(floatStorage.size(), 0);
        }
    protected:
        const IFArchetype::StorageType<int32_t>& intStorage{this->storage<int32_t>()};
        const IFArchetype::StorageType<float>& floatStorage{this->storage<float>()};
    };
} // common

#endif // COMMON_HPP
