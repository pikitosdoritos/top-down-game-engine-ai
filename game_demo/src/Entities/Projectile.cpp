#include "Entities/Projectile.hpp"
#include "engine/Core/GameEngine.hpp"
#include "engine/Components/TransformComponent.hpp"
#include "engine/Components/VelocityComponent.hpp"
#include "engine/Components/ColliderComponent.hpp"
#include "engine/Components/HealthComponent.hpp"

using namespace engine;

Projectile::Projectile(Vec2f origin, Vec2f direction,
                        float speed, float dmg, float lifetime)
    : Entity("projectile"), damage(dmg), m_lifetime(lifetime)
{
    addComponent<TransformComponent>(origin);

    auto& vel = addComponent<VelocityComponent>();
    vel.velocity = direction * speed;

    // Trigger collider — no physical resolution, just callback
    auto& col = addComponent<ColliderComponent>(Vec2f{10.f, 10.f});
    col.isTrigger = true;
    col.layer = CollisionLayer::Projectile;
    col.mask  = CollisionLayer::Enemy;

    col.onCollision = [this](Entity& other) {
        if (destroyed) return; // already hit something
        if (auto* hp = other.getComponent<HealthComponent>()) {
            hp->takeDamage(damage);
        }
        destroyed = true;
    };

    // Glowing orb appearance
    m_shape.setRadius(5.f);
    m_shape.setOrigin({5.f, 5.f});
    m_shape.setFillColor(sf::Color(255, 160, 30));
    m_shape.setOutlineColor(sf::Color(255, 230, 120));
    m_shape.setOutlineThickness(2.f);
}

void Projectile::update(GameEngine& engine, float dt)
{
    if (!active) return;

    m_elapsed += dt;
    if (m_elapsed >= m_lifetime) {
        destroyed = true;
        return;
    }

    auto* tf  = getComponent<TransformComponent>();
    auto* vel = getComponent<VelocityComponent>();
    if (tf && vel) tf->position += vel->velocity * dt;

    Entity::update(engine, dt);
}

void Projectile::render(sf::RenderWindow& window)
{
    auto* tf = getComponent<TransformComponent>();
    if (!tf) return;

    // Fade out in last 0.3 s
    float remaining = m_lifetime - m_elapsed;
    uint8_t alpha = (remaining < 0.3f)
        ? static_cast<uint8_t>(255.f * (remaining / 0.3f))
        : 255;

    auto fill    = sf::Color(255, 160, 30, alpha);
    auto outline = sf::Color(255, 230, 120, alpha);
    m_shape.setFillColor(fill);
    m_shape.setOutlineColor(outline);
    m_shape.setPosition(tf->position);
    window.draw(m_shape);
}
