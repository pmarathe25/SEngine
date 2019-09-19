#include "meta/helpers.hpp"
#include "archetype.hpp"
#include <STest.hpp>

using Stealth::Engine::Archetype;
using Stealth::Engine::removeCVRef;

using IFArchetype = Archetype<int32_t, float>;

STEST(CanConstruct) {
    IFArchetype archetype{};
}

namespace InternalTests {
    constexpr Stealth::Engine::EntityID testEntity{0};

    STEST_F(IFArchetype, CanGetVectors) {
        EXPECT_TRUE((std::is_same_v<removeCVRef<decltype(this->storage<int32_t>())>, IFArchetype::StorageType<int32_t>>));
        EXPECT_EQ(this->storage<int32_t>().size(), 0);
    }

    STEST_F(IFArchetype, CanAddComponentsInOrder) {
        constexpr int32_t i = 10;
        constexpr float f = 0.125f;
        this->addEntity(testEntity, i, f);
        EXPECT_EQ(this->size(), 1);
        EXPECT_EQ(this->storage<int32_t>().size(), 1);
        EXPECT_EQ(this->storage<int32_t>().at(0), i);
        EXPECT_EQ(this->storage<float>().size(), 1);
        EXPECT_EQ(this->storage<float>().at(0), f);
    }

    STEST_F(IFArchetype, CanAddComponentsOutOfOrder) {
        constexpr int32_t i = 10;
        constexpr float f = 0.125f;
        this->addEntity(testEntity, f, i);
        EXPECT_EQ(this->size(), 1);
        EXPECT_EQ(this->storage<int32_t>().size(), 1);
        EXPECT_EQ(this->storage<int32_t>().at(0), i);
        EXPECT_EQ(this->storage<float>().size(), 1);
        EXPECT_EQ(this->storage<float>().at(0), f);
    }

    STEST_F(IFArchetype, ContainsEntityWorks) {
        constexpr int32_t i = 10;
        constexpr float f = 0.125f;
        this->addEntity(testEntity, f, i);
        EXPECT_TRUE(this->containsEntity(testEntity));
        EXPECT_FALSE(this->containsEntity(testEntity.next()));
    }

    STEST_F(IFArchetype, CanRemoveEntity) {
        constexpr int32_t i = 10;
        constexpr float f = 0.125f;
        this->addEntity(testEntity, f, i);
        const auto [iRemoved, fRemoved] = this->removeEntity(testEntity);
        EXPECT_EQ(iRemoved, i);
        EXPECT_EQ(fRemoved, f);
        EXPECT_EQ(mSize, 0);
        EXPECT_EQ(this->storage<int32_t>().size(), 0);
        EXPECT_EQ(this->storage<float>().size(), 0);
        EXPECT_EQ(this->size(), 0);
    }
} // InternalTests

STEST(IdenticalArchetypesAreSame) {
    static_assert(std::is_same_v<Archetype<int32_t, float>, Archetype<int32_t, float>>, "Expected equality");
}

STEST(DifferentlyOrderedArchetypesAreSame) {
    static_assert(std::is_same_v<Archetype<float, int32_t>, Archetype<int32_t, float>>, "Expected equality");
}

STEST(DifferentArchetypesAreDifferent) {
    static_assert(!std::is_same_v<Archetype<float, uint8_t>, Archetype<float, int32_t>>, "Expected inequality");
}

STEST(ArchetypeContainsCorrectElements) {
    using IFDC = Archetype<int32_t, float, double, char>;
    static_assert(IFDC::ComponentPack::contains<char, double, float, int32_t>(), "Archetype does not contain correct components");
}

STEST_MAIN();
