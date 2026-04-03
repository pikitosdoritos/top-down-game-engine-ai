#include "Entities/Projectile.hpp"
#include "engine/Core/GameEngine.hpp"
#include "engine/Components/TransformComponent.hpp"
#include "engine/Components/VelocityComponent.hpp"
#include "engine/Components/ColliderComponent.hpp"
#include "engine/Components/HealthComponent.hpp"
#include <cmath>

using namespace engine;

Projectile::Projectile(Vec2f origin, Vec2f direction,
                       float speed, float dmg, float lifetime,
                       const sf::Texture* tex,
                       const sf::Texture* animTex)
    : Entity("projectile"), damage(dmg), m_lifetime(lifetime),
      m_animTex(animTex)
{
    m_direction = std::atan2(direction.y, direction.x);

    // Animated sheet takes priority; static sprite is fallback
    if (animTex) {
        m_animSprite.emplace(*animTex);
        m_animSprite->setTextureRect(sf::IntRect({0, 0}, {m_animFrameW, m_animFrameH}));
        m_animSprite->setOrigin({m_animFrameW / 2.f, m_animFrameH / 2.f});
        m_animSprite->setRotation(sf::degrees(m_direction * (180.f / 3.14159265f) + 45.f));
    } else if (tex) {
        m_sprite.emplace(*tex);
        m_sprite->setOrigin({tex->getSize().x / 2.f, tex->getSize().y / 2.f});
        m_sprite->setRotation(sf::degrees(m_direction * (180.f / 3.14159265f) + 45.f));
    }

    addComponent<TransformComponent>(origin);

    auto& vel = addComponent<VelocityComponent>();
    vel.velocity = direction * speed;

    auto& col = addComponent<ColliderComponent>(Vec2f{10.f, 10.f});
    col.isTrigger = true;
    col.layer = CollisionLayer::Projectile;
    col.mask  = CollisionLayer::Enemy;

    col.onCollision = [this](Entity& other) {
        if (destroyed) return;
        if (auto* hp = other.getComponent<HealthComponent>())
            hp->takeDamage(damage);
        destroyed = true;
    };

    // Fallback circle
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

    // Advance animation frame (loop through all 4 frames over lifetime)
    if (m_animSprite && m_animTex) {
        float frameDur = m_lifetime / (m_animFrames * 2.f); // cycle twice per lifetime
        m_animFrameTime += dt;
        if (m_animFrameTime >= frameDur) {
            m_animFrameTime -= frameDur;
            m_animFrame = (m_animFrame + 1) % m_animFrames;
            m_animSprite->setTextureRect(
                sf::IntRect({m_animFrame * m_animFrameW, 0}, {m_animFrameW, m_animFrameH}));
        }
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

    float remaining = m_lifetime - m_elapsed;
    uint8_t alpha = (remaining < 0.3f)
        ? static_cast<uint8_t>(255.f * (remaining / 0.3f))
        : 255;

    if (m_animSprite) {
        m_animSprite->setPosition(tf->position);
        auto col = m_animSprite->getColor();
        col.a = alpha;
        m_animSprite->setColor(col);
        window.draw(*m_animSprite);
    } else if (m_sprite) {
        m_sprite->setPosition(tf->position);
        auto col = m_sprite->getColor();
        col.a = alpha;
        m_sprite->setColor(col);
        window.draw(*m_sprite);
    } else {
        auto fill    = sf::Color(255, 160, 30, alpha);
        auto outline = sf::Color(255, 230, 120, alpha);
        m_shape.setFillColor(fill);
        m_shape.setOutlineColor(outline);
        m_shape.setPosition(tf->position);
        window.draw(m_shape);
    }
}
