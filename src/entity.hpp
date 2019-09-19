#ifndef ENTITY_HPP
#define ENTITY_HPP
#include <cstddef> // For std::size_t
#include <utility> // For std::move
#include <functional> // For std::hash

namespace Stealth::Engine {
    // Using a struct allows for better type checking compared to a using declaration.
    class EntityID {
    protected:
        std::size_t mID;
    public:
        using UnderlyingType = std::size_t;

        EntityID() = delete;
        explicit constexpr EntityID(std::size_t id) noexcept : mID{std::move(id)} { }

        explicit operator UnderlyingType() const { return mID; }

        constexpr EntityID next() const noexcept {
            return EntityID{mID + 1};
        }

        constexpr bool operator==(const EntityID& other) const noexcept {
            return mID == other.mID;
        }
    };

    class Entity {
    public:
        explicit constexpr Entity(EntityID id) : mID{std::move(id)} { }

        constexpr const EntityID& id() const noexcept {
            return mID;
        }
    protected:
        EntityID mID;
    };
} // Stealth::Engine

namespace std {
    template <>
    struct hash<Stealth::Engine::EntityID> : public hash<std::size_t> {
        using argument_type = Stealth::Engine::EntityID;
        using Base = hash<std::size_t>;

        Base::result_type operator()(const argument_type& key) const noexcept {
            return Base{}(static_cast<std::size_t>(key));
        }
    };
} // std

#endif // ENTITY_HPP
