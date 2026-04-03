#pragma once
#include "engine/Entity/Entity.hpp"
#include "engine/Core/Types.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/View.hpp>
#include <memory>
#include <vector>

namespace engine { class GameEngine; }

class Player : public engine::Entity {
public:
    Player(const sf::Texture* tex = nullptr, const sf::Texture* swordTex = nullptr);

    void update(engine::GameEngine& engine, float dt) override;
    void render(sf::RenderWindow& window) override;

    bool isAttacking() const { return m_attacking; }
    engine::FloatRect attackHitbox() const;

    // Called by GameScene each frame so handleAttack can use correct world coords
    void setCameraView(const sf::View& v) { m_cameraView = v; }

    // Entities queued for spawn (drained by GameScene each frame)
    std::vector<std::unique_ptr<engine::Entity>> pendingSpawns;

    float moveSpeed    = 180.f;
    float attackDamage = 25.f;

private:
    void handleMovement(engine::GameEngine& engine);
    void handleAttack(engine::GameEngine& engine, float dt, const sf::View& cameraView);
    void updateVisuals();

    sf::RectangleShape m_body;
    sf::RectangleShape m_sword;
    sf::CircleShape m_shadow;
    sf::CircleShape m_glow;

    enum class State { Idle, Walk, Attack, Dead };
    State m_state          = State::Idle;
    float m_attackTimer    = 0.f;
    float m_attackDuration = 0.25f;
    float m_attackCooldown = 0.f;
    bool  m_attacking      = false;
    engine::Vec2f m_facing {1.f, 0.f};
    sf::View  m_cameraView;
    std::optional<sf::Sprite> m_sprite;
    std::optional<sf::Sprite> m_swordSprite;
};
