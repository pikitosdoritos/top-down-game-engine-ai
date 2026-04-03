#pragma once
#include "engine/Entity/Entity.hpp"
#include "engine/Core/Types.hpp"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <optional>

namespace engine { class GameEngine; }

class Projectile : public engine::Entity {
public:
    // tex      = static bullet sprite (bullet.png)
    // animTex  = animated sprite sheet (bullet_anim.png, 4 frames 32x32)
    Projectile(engine::Vec2f origin, engine::Vec2f direction,
               float speed = 400.f, float damage = 20.f, float lifetime = 1.5f,
               const sf::Texture* tex = nullptr,
               const sf::Texture* animTex = nullptr);

    void update(engine::GameEngine& engine, float dt) override;
    void render(sf::RenderWindow& window) override;

    float damage = 20.f;

private:
    sf::CircleShape           m_shape;
    float                     m_lifetime;
    float                     m_elapsed    = 0.f;
    float                     m_direction  = 0.f;  // radians, cached at spawn
    std::optional<sf::Sprite> m_sprite;             // static fallback

    // Animation
    const sf::Texture*        m_animTex       = nullptr;
    std::optional<sf::Sprite> m_animSprite;
    int   m_animFrames    = 4;
    int   m_animFrameW    = 32;
    int   m_animFrameH    = 32;
    float m_animFrameTime = 0.f;
    int   m_animFrame     = 0;
};
