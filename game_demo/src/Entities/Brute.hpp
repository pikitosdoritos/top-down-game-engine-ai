#pragma once
#include "engine/Entity/Entity.hpp"
#include "engine/Core/Types.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <optional>

namespace engine { class GameEngine; }

// ─────────────────────────────────────────────────────────────────────────────
// Brute — heavy, slow enemy introduced in Level 2.
// Larger hitbox, more HP, slower but hits much harder.
// Visually distinct: darker purple body, horns drawn as upward triangles.
// ─────────────────────────────────────────────────────────────────────────────
class Brute : public engine::Entity {
public:
    Brute(engine::Vec2f spawnPos,
          float hp          = 140.f,
          float speed       = 50.f,
          float damage      = 22.f,
          const sf::Texture* tex = nullptr);

    void update(engine::GameEngine& engine, float dt) override;
    void render(sf::RenderWindow& window) override;

    bool isDead() const;

private:
    void updateVisuals();

    sf::RectangleShape m_body;
    sf::RectangleShape m_horn1, m_horn2;   // cosmetic horns
    sf::RectangleShape m_hpBarBg;
    sf::RectangleShape m_hpBarFill;
    sf::CircleShape    m_shadow;

    float m_hurtFlash  = 0.f;
    float m_deathTimer = 1.0f;   // brutes take longer to vanish
    std::optional<sf::Sprite> m_sprite;
};
