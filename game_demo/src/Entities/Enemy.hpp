#pragma once
#include "engine/Entity/Entity.hpp"
#include "engine/Core/Types.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace engine { class GameEngine; }

namespace sf { class Texture; }

class Enemy : public engine::Entity {
public:
    Enemy(engine::Vec2f spawnPos, float hp = 60.f, const sf::Texture* tex = nullptr);

    void update(engine::GameEngine& engine, float dt) override;
    void render(sf::RenderWindow& window) override;

    bool isDead() const;

private:
    void updateVisuals();

    sf::RectangleShape m_body;
    sf::RectangleShape m_hpBarBg;
    sf::RectangleShape m_hpBarFill;
    sf::CircleShape    m_shadow;

    float m_hurtFlash  = 0.f;   // seconds remaining of hurt flash
    float m_deathTimer = 0.6f;  // linger before destroying
    std::optional<sf::Sprite> m_sprite;
};
