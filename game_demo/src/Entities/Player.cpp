#include "Entities/Player.hpp"
#include "Entities/Projectile.hpp"
#include "engine/Core/GameEngine.hpp"
#include "engine/Components/TransformComponent.hpp"
#include "engine/Components/VelocityComponent.hpp"
#include "engine/Components/HealthComponent.hpp"
#include "engine/Components/ColliderComponent.hpp"
#include <cmath>

using namespace engine;

static float vecLen(Vec2f v) { return std::sqrt(v.x * v.x + v.y * v.y); }
static Vec2f vecNorm(Vec2f v) {
    float l = vecLen(v);
    return l > 0.f ? Vec2f{v.x / l, v.y / l} : Vec2f{1.f, 0.f};
}

Player::Player() : Entity("player")
{
    addComponent<TransformComponent>(Vec2f{0.f, 0.f});
    addComponent<VelocityComponent>();
    addComponent<HealthComponent>(100.f);

    auto& col = addComponent<ColliderComponent>(Vec2f{26.f, 26.f});
    col.layer = CollisionLayer::Player;
    col.mask  = CollisionLayer::Enemy | CollisionLayer::Wall;

    m_body.setSize({26.f, 26.f});
    m_body.setOrigin({13.f, 13.f});
    m_body.setFillColor(sf::Color(70, 90, 180));
    m_body.setOutlineColor(sf::Color(140, 160, 255));
    m_body.setOutlineThickness(2.f);

    // Sword stub drawn in facing direction
    m_sword.setSize({20.f, 5.f});
    m_sword.setOrigin({0.f, 2.5f});
    m_sword.setFillColor(sf::Color(220, 200, 80));
    m_sword.setOutlineColor(sf::Color(255, 240, 160));
    m_sword.setOutlineThickness(1.f);
}

void Player::update(GameEngine& engine, float dt)
{
    if (!active) return;

    auto* hp = getComponent<HealthComponent>();
    if (hp && hp->isDead()) {
        m_state = State::Dead;
        getComponent<VelocityComponent>()->velocity = {0.f, 0.f};
        return;
    }

    handleMovement(engine);
    handleAttack(engine, dt);

    // Integrate position
    auto* tf  = getComponent<TransformComponent>();
    auto* vel = getComponent<VelocityComponent>();
    if (tf && vel) tf->position += vel->velocity * dt;

    updateVisuals();
    Entity::update(engine, dt);
}

void Player::handleMovement(GameEngine& engine)
{
    auto& input = engine.input();
    Vec2f dir{0.f, 0.f};

    if (input.isKeyDown(sf::Keyboard::Key::W)) dir.y -= 1.f;
    if (input.isKeyDown(sf::Keyboard::Key::S)) dir.y += 1.f;
    if (input.isKeyDown(sf::Keyboard::Key::A)) dir.x -= 1.f;
    if (input.isKeyDown(sf::Keyboard::Key::D)) dir.x += 1.f;

    float l = vecLen(dir);
    if (l > 0.f) {
        dir = {dir.x / l, dir.y / l};
        m_facing = dir;
        if (m_state != State::Attack) m_state = State::Walk;
    } else {
        if (m_state != State::Attack) m_state = State::Idle;
    }

    auto* vel = getComponent<VelocityComponent>();
    if (vel) vel->velocity = dir * moveSpeed;
}

void Player::handleAttack(GameEngine& engine, float dt)
{
    m_attackCooldown -= dt;
    m_attacking = false;

    // Count down active swing
    if (m_attackTimer > 0.f) {
        m_attackTimer -= dt;
        m_state      = State::Attack;
        m_attacking  = true;
        return;
    }

    if (m_attackCooldown > 0.f) return;

    // Left mouse → fire projectile
    if (engine.input().isMousePressed(sf::Mouse::Button::Left)) {
        auto* tf = getComponent<TransformComponent>();
        if (tf) {
            Vec2f mouse = engine.input().mouseWorldPosition(engine.window().sfWindow());
            Vec2f dir   = vecNorm(mouse - tf->position);
            m_facing = dir;
            pendingSpawns.push_back(
                std::make_unique<Projectile>(tf->position, dir, 420.f, attackDamage, 1.8f));
        }
        m_attackCooldown = 0.22f;
        m_attackTimer    = 0.12f;
        m_state          = State::Attack;
        m_attacking      = true;
    }

    // F key → melee swing (hitbox applied by GameScene)
    if (engine.input().isKeyPressed(sf::Keyboard::Key::F)) {
        auto* tf = getComponent<TransformComponent>();
        if (tf) {
            Vec2f mouse = engine.input().mouseWorldPosition(engine.window().sfWindow());
            m_facing = vecNorm(mouse - tf->position);
        }
        m_attackCooldown = 0.45f;
        m_attackTimer    = 0.18f;
        m_state          = State::Attack;
        m_attacking      = true;
    }
}

void Player::updateVisuals()
{
    auto* hp = getComponent<HealthComponent>();
    float ratio = hp ? hp->ratio() : 1.f;

    if (m_state == State::Dead) {
        m_body.setFillColor(sf::Color(30, 30, 60));
        m_body.setOutlineColor(sf::Color(60, 60, 100));
    } else if (m_state == State::Attack) {
        m_body.setFillColor(sf::Color(110, 130, 240));
    } else {
        auto r = static_cast<uint8_t>(70  + static_cast<uint8_t>((1.f - ratio) * 80));
        auto b = static_cast<uint8_t>(180 * ratio);
        m_body.setFillColor(sf::Color(r, 90, b));
    }
}

void Player::render(sf::RenderWindow& window)
{
    auto* tf = getComponent<TransformComponent>();
    if (!tf) return;

    sf::Vector2f pos = tf->position;
    m_body.setPosition(pos);
    window.draw(m_body);

    if (m_state != State::Dead) {
        float angle = std::atan2(m_facing.y, m_facing.x) * (180.f / 3.14159265f);
        m_sword.setPosition({pos.x + m_facing.x * 14.f,
                              pos.y + m_facing.y * 14.f});
        m_sword.setRotation(sf::degrees(angle));
        window.draw(m_sword);
    }
}

FloatRect Player::attackHitbox() const
{
    auto* tf = getComponent<TransformComponent>();
    if (!tf) return {{0.f, 0.f}, {0.f, 0.f}};
    Vec2f center = tf->position + m_facing * 38.f;
    return {{center.x - 22.f, center.y - 22.f}, {44.f, 44.f}};
}
