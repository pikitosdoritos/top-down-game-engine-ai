#pragma once
#include "engine/Scene/IScene.hpp"
#include "engine/Camera/Camera.hpp"
#include "engine/Tilemap/Tilemap.hpp"
#include "engine/Tilemap/FogOfWar.hpp"
#include "engine/Collision/CollisionSystem.hpp"
#include "engine/UI/HealthBar.hpp"
#include "engine/UI/Label.hpp"
#include "engine/Entity/Entity.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Color.hpp>
#include <vector>
#include <memory>
#include <optional>

namespace engine { class GameEngine; }
class Player;

// ─────────────────────────────────────────────────────────────────────────────
// CampaignLevelScene
//
// A single reusable scene that drives all 3 campaign levels.
// It reads LevelConfig from CampaignState::current() on enter,
// so swapping from Level1 → Level2 → Level3 is just a SceneID switch.
// ─────────────────────────────────────────────────────────────────────────────
class CampaignLevelScene : public engine::IScene {
public:
    void onEnter(engine::GameEngine& engine)  override;
    void onExit(engine::GameEngine& engine)   override;
    void handleEvent(engine::GameEngine& engine, const sf::Event& event) override;
    void update(engine::GameEngine& engine, float dt) override;
    void render(engine::GameEngine& engine) override;

private:
    // Setup helpers
    void buildMap(engine::GameEngine& engine);
    void spawnPlayer(engine::GameEngine& engine);
    void spawnEnemies(engine::GameEngine& engine);
    void attachAttackCallback(engine::Entity& enemy, float damage);
    void handleTileCollisions();
    void pruneDeadEntities();

    engine::Camera          m_camera;
    engine::Tilemap         m_tilemap;
    engine::FogOfWar        m_fog;
    engine::CollisionSystem m_collision;

    std::vector<std::unique_ptr<engine::Entity>> m_entities;
    Player* m_player = nullptr;     // non-owning

    // HUD
    std::optional<engine::HealthBar> m_hpBar;
    std::optional<engine::Label>     m_hpLabel;
    std::optional<engine::Label>     m_levelLabel;
    std::optional<engine::Label>     m_objectiveLabel;
    std::optional<engine::Label>     m_killLabel;
    std::optional<engine::Label>     m_transitionLabel;

    int   m_killCount    = 0;
    int   m_totalEnemies = 0;
    bool  m_playerDead   = false;
    bool  m_victory      = false;
    float m_endTimer     = 0.f;

    // Ambient clear colour read from config
    sf::Color m_ambientColor;
};
