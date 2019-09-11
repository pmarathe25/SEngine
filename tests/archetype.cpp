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
    STEST_F(IFArchetype, CanGetVectors) {
        EXPECT_TRUE((std::is_same_v<removeCVRef<decltype(this->storage<int32_t>())>, IFArchetype::StorageType<int32_t>>));
        EXPECT_EQ(this->storage<int32_t>().size(), 0);
    }

    STEST_F(IFArchetype, CanAddComponentsInOrder) {
        constexpr int32_t i = 10;
        constexpr float f = 0.125f;
        this->addComponents(i, f);
        EXPECT_EQ(this->storage<int32_t>().size(), 1);
        EXPECT_EQ(this->storage<int32_t>().at(0), i);
        EXPECT_EQ(this->storage<float>().size(), 1);
        EXPECT_EQ(this->storage<float>().at(0), f);
    }

    STEST_F(IFArchetype, CanAddComponentsOutOfOrder) {
        constexpr int32_t i = 10;
        constexpr float f = 0.125f;
        this->addComponents(f, i);
        EXPECT_EQ(this->storage<int32_t>().size(), 1);
        EXPECT_EQ(this->storage<int32_t>().at(0), i);
        EXPECT_EQ(this->storage<float>().size(), 1);
        EXPECT_EQ(this->storage<float>().at(0), f);
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

class PopulatedIFFixture {
public:
    PopulatedIFFixture() {
        // These will each be reordered to (int32_t, float)
        archetype.addComponents(0.f, 1);
        archetype.addComponents(67, 1.f);
        archetype.addComponents(0, 3.14f);
    }
protected:
    IFArchetype archetype{};
};

STEST_F(PopulatedIFFixture, CanGetPackOfReferencesToComponents) {
    static_assert(std::is_same_v<decltype(archetype.at(0)), Stealth::Engine::Pack<const int32_t&, const float&>>, "at() should return a Pack of references");
    const auto& [i, f] = archetype.at(0);
    EXPECT_EQ(i, 1);
    EXPECT_EQ(f, 0.f);
}

STEST_F(PopulatedIFFixture, CanGetReorderedPackOfReferencesToComponents) {
    const auto& [f, i] = archetype.at<float, int32_t>(1);
    EXPECT_EQ(f, 1.f);
    EXPECT_EQ(i, 67);
}

STEST_F(PopulatedIFFixture, CanGetPackOfSelectedReferencesToComponents) {
    const auto& [f] = archetype.at<float>(1);
    EXPECT_EQ(f, 1.f);
}

STEST_MAIN();
