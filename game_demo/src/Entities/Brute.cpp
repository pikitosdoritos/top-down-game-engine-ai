#include "Entities/Brute.hpp"
#include "engine/Core/GameEngine.hpp"
#include "engine/Components/TransformComponent.hpp"
#include "engine/Components/VelocityComponent.hpp"
#include "engine/Components/HealthComponent.hpp"
#include "engine/Components/ColliderComponent.hpp"
#include "engine/Components/AIComponent.hpp"
#include <SFML/Graphics/Texture.hpp>
#include <cmath>
#include <algorithm>

using namespace engine;

Brute::Brute(Vec2f spawnPos, float hp, float speed, float damage,
             const sf::Texture* tex)
    : Entity("enemy")   // tag still "enemy" so GameScene's kill counter works
{
    if (tex) {
        m_sprite.emplace(*tex);
        m_sprite->setOrigin({tex->getSize().x / 2.f, tex->getSize().y / 2.f});
    }

    addComponent<TransformComponent>(spawnPos);
    addComponent<VelocityComponent>();

    auto& health = addComponent<HealthComponent>(hp);
    health.onDamaged = [this](float) { m_hurtFlash = 0.22f; };

    auto& col = addComponent<ColliderComponent>(Vec2f{38.f, 38.f});
    col.layer = CollisionLayer::Enemy;
    col.mask  = CollisionLayer::Player | CollisionLayer::Projectile | CollisionLayer::Wall;

    auto& ai = addComponent<AIComponent>();
    ai.detectionRange = 460.f;
    ai.attackRange    = 42.f;
    ai.moveSpeed      = speed;
    ai.attackDamage   = damage;
    ai.attackCooldown = 1.6f;  // slow but powerful

    // Large body — dark purple/blue
    m_body.setSize({38.f, 38.f});
    m_body.setOrigin({19.f, 19.f});
    m_body.setFillColor(sf::Color(80, 20, 120));
    m_body.setOutlineColor(sf::Color(160, 60, 220));
    m_body.setOutlineThickness(3.f);

    // Horns
    m_horn1.setSize({6.f, 16.f});
    m_horn1.setOrigin({3.f, 16.f});
    m_horn1.setFillColor(sf::Color(200, 80, 255));

    m_horn2.setSize({6.f, 16.f});
    m_horn2.setOrigin({3.f, 16.f});
    m_horn2.setFillColor(sf::Color(200, 80, 255));

    // Shadow — larger than grunt
    m_shadow.setRadius(20.f);
    m_shadow.setOrigin({20.f, 10.f});
    m_shadow.setScale({1.f, 0.5f});
    m_shadow.setFillColor(sf::Color(0, 0, 0, 140));

    // HP bar
    m_hpBarBg.setSize({44.f, 6.f});
    m_hpBarBg.setFillColor(sf::Color(30, 10, 50));
    m_hpBarBg.setOutlineColor(sf::Color(0, 0, 0));
    m_hpBarBg.setOutlineThickness(1.f);

    m_hpBarFill.setSize({44.f, 6.f});
    m_hpBarFill.setFillColor(sf::Color(160, 40, 220));
}

void Brute::update(GameEngine& engine, float dt)
{
    if (!active) return;

    auto* hp = getComponent<HealthComponent>();
    if (hp && hp->isDead()) {
        getComponent<VelocityComponent>()->velocity = {0.f, 0.f};
        m_deathTimer -= dt;
        if (m_deathTimer <= 0.f) destroyed = true;
        updateVisuals();
        return;
    }

    m_hurtFlash -= dt;
    Entity::update(engine, dt);

    auto* tf  = getComponent<TransformComponent>();
    auto* vel = getComponent<VelocityComponent>();
    if (tf && vel) tf->position += vel->velocity * dt;

    updateVisuals();
}

bool Brute::isDead() const
{
    auto* hp = getComponent<HealthComponent>();
    return hp && hp->isDead();
}

void Brute::updateVisuals()
{
    auto* hp = getComponent<HealthComponent>();
    float ratio = hp ? hp->ratio() : 0.f;
    m_hpBarFill.setSize({44.f * ratio, 6.f});

    auto* ai = getComponent<AIComponent>();
    bool justAttacked = (ai && ai->state == AIState::Attack && ai->attackTimer > ai->attackCooldown - 0.2f);

    if (isDead()) {
        m_body.setFillColor(sf::Color(30, 10, 40));
        m_body.setOutlineColor(sf::Color(50, 20, 60));
    } else if (m_hurtFlash > 0.f) {
        m_body.setFillColor(sf::Color(255, 180, 255));
    } else if (justAttacked) {
        m_body.setFillColor(sf::Color(255, 255, 100)); // flash yellow
    } else {
        auto r = static_cast<uint8_t>(80 * ratio);
        m_body.setFillColor(sf::Color(r, 20, 120));
    }
}

void Brute::render(sf::RenderWindow& window)
{
    auto* tf = getComponent<TransformComponent>();
    if (!tf) return;

    sf::Vector2f pos = tf->position;

    if (!isDead()) {
        m_shadow.setPosition({pos.x, pos.y + 18.f});
        window.draw(m_shadow);
    }

    if (m_sprite) {
        m_sprite->setPosition(pos);
        
        auto* ai = getComponent<AIComponent>();
        bool justAttacked = (ai && ai->state == AIState::Attack && ai->attackTimer > ai->attackCooldown - 0.3f);
        
        if (m_hurtFlash > 0.f) m_sprite->setColor(sf::Color(255, 150, 255));
        else if (justAttacked) m_sprite->setColor(sf::Color(255, 255, 100));
        else m_sprite->setColor(sf::Color::White);
        
        window.draw(*m_sprite);
    } else {
        m_body.setPosition(pos);
        window.draw(m_body);

        // Draw horns above the body
        if (!isDead()) {
            m_horn1.setPosition({pos.x - 10.f, pos.y - 16.f});
            m_horn2.setPosition({pos.x + 10.f, pos.y - 16.f});
            m_horn1.setRotation(sf::degrees(-15.f));
            m_horn2.setRotation(sf::degrees(15.f));
            window.draw(m_horn1);
            window.draw(m_horn2);
        }
    }

    // HP bar (only if alive and damaged)
    auto* hp = getComponent<HealthComponent>();
    if (hp && hp->isAlive() && hp->ratio() < 1.f) {
        sf::Vector2f barPos = {pos.x - 22.f, pos.y - 28.f};
        m_hpBarBg.setPosition(barPos);
        m_hpBarFill.setPosition(barPos);
        window.draw(m_hpBarBg);
        window.draw(m_hpBarFill);
    }
}
