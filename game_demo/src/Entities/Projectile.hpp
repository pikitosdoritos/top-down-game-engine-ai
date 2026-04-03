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
    Projectile(engine::Vec2f origin, engine::Vec2f direction,
               float speed = 400.f, float damage = 20.f, float lifetime = 1.5f,
               const sf::Texture* tex = nullptr);

    void update(engine::GameEngine& engine, float dt) override;
    void render(sf::RenderWindow& window) override;

    float damage = 20.f;

private:
    sf::CircleShape m_shape;
    float           m_lifetime;
    float           m_elapsed = 0.f;
    std::optional<sf::Sprite> m_sprite;
};
