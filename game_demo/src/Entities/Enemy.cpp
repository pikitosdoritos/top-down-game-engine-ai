#include "Entities/Enemy.hpp"
#include "engine/Core/GameEngine.hpp"
#include "engine/Components/TransformComponent.hpp"
#include "engine/Components/VelocityComponent.hpp"
#include "engine/Components/HealthComponent.hpp"
#include "engine/Components/ColliderComponent.hpp"
#include "engine/Components/AIComponent.hpp"
#include <cmath>
#include <algorithm>

#include <SFML/Graphics/Texture.hpp>

using namespace engine;

Enemy::Enemy(Vec2f spawnPos, float hp, const sf::Texture* tex) : Entity("enemy")
{
    if (tex) {
        m_sprite.emplace(*tex);
        m_sprite->setOrigin({tex->getSize().x / 2.f, tex->getSize().y / 2.f});
    }

    auto& tf = addComponent<TransformComponent>(spawnPos);
    (void)tf;
    addComponent<VelocityComponent>();

    auto& health = addComponent<HealthComponent>(hp);
    health.onDamaged = [this](float) {
        m_hurtFlash = 0.18f;
    };
    health.onDeath = [this]() {
        // Velocity zeroed in update when dead is detected
    };

    auto& col = addComponent<ColliderComponent>(Vec2f{28.f, 28.f});
    col.layer = CollisionLayer::Enemy;
    col.mask  = CollisionLayer::Player | CollisionLayer::Projectile | CollisionLayer::Wall | CollisionLayer::Enemy;

    auto& ai = addComponent<AIComponent>();
    ai.detectionRange = 380.f;
    ai.attackRange    = 32.f;
    ai.moveSpeed      = 85.f;
    ai.attackDamage   = 12.f;
    ai.attackCooldown = 1.0f;

    // Body
    m_body.setSize({28.f, 28.f});
    m_body.setOrigin({14.f, 14.f});
    m_body.setFillColor(sf::Color(160, 30, 30));
    m_body.setOutlineColor(sf::Color(220, 80, 80));
    m_body.setOutlineThickness(2.f);

    m_shadow.setRadius(14.f);
    m_shadow.setOrigin({14.f, 7.f});
    m_shadow.setScale({1.f, 0.5f});
    m_shadow.setFillColor(sf::Color(0, 0, 0, 110));

    // HP bar (world-space, above entity)
    m_hpBarBg.setSize({32.f, 5.f});
    m_hpBarBg.setFillColor(sf::Color(50, 20, 20));
    m_hpBarBg.setOutlineColor(sf::Color(0, 0, 0));
    m_hpBarBg.setOutlineThickness(1.f);

    m_hpBarFill.setSize({32.f, 5.f});
    m_hpBarFill.setFillColor(sf::Color(200, 30, 30));
}

void Enemy::update(GameEngine& engine, float dt)
{
    if (!active) return;

    auto* hp = getComponent<HealthComponent>();
    if (hp && hp->isDead()) {
        // Linger a moment then mark destroyed
        getComponent<VelocityComponent>()->velocity = {0.f, 0.f};
        m_deathTimer -= dt;
        if (m_deathTimer <= 0.f) destroyed = true;
        updateVisuals();
        return;
    }

    m_hurtFlash -= dt;

    // Velocity will be set by AIComponent::update (called via Entity::update)
    Entity::update(engine, dt);

    // Integrate position
    auto* tf  = getComponent<TransformComponent>();
    auto* vel = getComponent<VelocityComponent>();
    if (tf && vel) tf->position += vel->velocity * dt;

    updateVisuals();
}

bool Enemy::isDead() const
{
    auto* hp = getComponent<HealthComponent>();
    return hp && hp->isDead();
}

void Enemy::updateVisuals()
{
    auto* hp = getComponent<HealthComponent>();
    float ratio = hp ? hp->ratio() : 0.f;
    m_hpBarFill.setSize({32.f * ratio, 5.f});

    auto* ai = getComponent<AIComponent>();
    bool justAttacked = (ai && ai->state == AIState::Attack && ai->attackTimer > ai->attackCooldown - 0.2f);

    if (isDead()) {
        m_body.setFillColor(sf::Color(80, 20, 20));
        m_body.setOutlineColor(sf::Color(100, 30, 30));
    } else if (m_hurtFlash > 0.f) {
        m_body.setFillColor(sf::Color(255, 150, 150));
    } else if (justAttacked) {
        m_body.setFillColor(sf::Color(255, 255, 100)); // Flash yellow when attacking!
    } else {
        auto r = static_cast<uint8_t>(160 * ratio);
        m_body.setFillColor(sf::Color(r, 30, 30));
    }
}

void Enemy::render(sf::RenderWindow& window)
{
    auto* tf = getComponent<TransformComponent>();
    if (!tf) return;

    sf::Vector2f pos = tf->position;

    if (!isDead()) {
        m_shadow.setPosition({pos.x, pos.y + 14.f});
        window.draw(m_shadow);
    }

    if (m_sprite) {
        m_sprite->setPosition(pos);
        
        auto* ai = getComponent<AIComponent>();
        bool justAttacked = (ai && ai->state == AIState::Attack && ai->attackTimer > ai->attackCooldown - 0.2f);
        
        if (isDead()) m_sprite->setColor(sf::Color(80, 80, 80));
        else if (m_hurtFlash > 0.f) m_sprite->setColor(sf::Color(255, 150, 150));
        else if (justAttacked) m_sprite->setColor(sf::Color(255, 255, 100));
        else m_sprite->setColor(sf::Color::White);
        
        window.draw(*m_sprite);
    } else {
        m_body.setPosition(pos);
        window.draw(m_body);
    }

    // Draw HP bar above entity (only while alive and damaged)
    auto* hp = getComponent<HealthComponent>();
    if (hp && hp->isAlive() && hp->ratio() < 1.f) {
        sf::Vector2f barPos = {pos.x - 16.f, pos.y - 22.f};
        m_hpBarBg.setPosition(barPos);
        m_hpBarFill.setPosition(barPos);
        window.draw(m_hpBarBg);
        window.draw(m_hpBarFill);
    }
}
