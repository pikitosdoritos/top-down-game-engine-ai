#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <cstdint>
#include <string>

namespace engine {

// Convenience aliases
using Vec2f  = sf::Vector2f;
using Vec2i  = sf::Vector2i;
using Vec2u  = sf::Vector2u;
using FloatRect = sf::FloatRect;

// Scene identifiers — extend freely in your game
enum class SceneID : uint8_t {
    None = 0,
    Menu,
    Game,
    GameOver,
};

// Collision layers (bitmask)
enum class CollisionLayer : uint16_t {
    None    = 0,
    Player  = 1 << 0,
    Enemy   = 1 << 1,
    Wall    = 1 << 2,
    Pickup  = 1 << 3,
    Projectile = 1 << 4,
};

inline CollisionLayer operator|(CollisionLayer a, CollisionLayer b) {
    return static_cast<CollisionLayer>(
        static_cast<uint16_t>(a) | static_cast<uint16_t>(b));
}
inline bool operator&(CollisionLayer a, CollisionLayer b) {
    return (static_cast<uint16_t>(a) & static_cast<uint16_t>(b)) != 0;
}

} // namespace engine
