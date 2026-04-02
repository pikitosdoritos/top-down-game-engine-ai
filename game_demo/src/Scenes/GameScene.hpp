#pragma once
#include "engine/Scene/IScene.hpp"
#include "engine/Camera/Camera.hpp"
#include "engine/Tilemap/Tilemap.hpp"
#include "engine/Collision/CollisionSystem.hpp"
#include "engine/UI/HealthBar.hpp"
#include "engine/UI/Label.hpp"
#include "engine/Entity/Entity.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Event.hpp>
#include <vector>
#include <memory>
#include <optional>

namespace engine { class GameEngine; }
class Player;
class Enemy;

class GameScene : public engine::IScene {
public:
    void onEnter(engine::GameEngine& engine) override;
    void onExit(engine::GameEngine& engine)  override;
    void handleEvent(engine::GameEngine& engine, const sf::Event& event) override;
    void update(engine::GameEngine& engine, float dt) override;
    void render(engine::GameEngine& engine) override;

private:
    void spawnEnemies(engine::GameEngine& engine);
    void spawnPlayer(engine::GameEngine& engine);
    void handleTileCollisions();
    void pruneDeadEntities();
    bool allEnemiesDead() const;

    engine::Camera          m_camera;
    engine::Tilemap         m_tilemap;
    engine::CollisionSystem m_collision;

    std::vector<std::unique_ptr<engine::Entity>> m_entities;

    Player* m_player = nullptr;  // non-owning, owned by m_entities

    // HUD
    std::optional<engine::HealthBar> m_playerHPBar;
    std::optional<engine::Label>     m_hpLabel;
    std::optional<engine::Label>     m_killLabel;

    int  m_killCount      = 0;
    int  m_totalEnemies   = 0;
    bool m_playerDead     = false;
    bool m_victory        = false;
    float m_endTimer      = 0.f; // delay before switching scene
};
