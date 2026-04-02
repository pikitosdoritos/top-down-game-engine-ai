#pragma once
#include "engine/Entity/Entity.hpp"
#include "engine/Core/Types.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

namespace engine { class GameEngine; }

class Enemy : public engine::Entity {
public:
    Enemy(engine::Vec2f spawnPos, float hp = 60.f);

    void update(engine::GameEngine& engine, float dt) override;
    void render(sf::RenderWindow& window) override;

    bool isDead() const;

private:
    void updateVisuals();

    sf::RectangleShape m_body;
    sf::RectangleShape m_hpBarBg;
    sf::RectangleShape m_hpBarFill;

    float m_hurtFlash  = 0.f;   // seconds remaining of hurt flash
    float m_deathTimer = 0.6f;  // linger before destroying
};
