#include "engine/Engine.hpp"
#include "Scenes/MenuScene.hpp"
#include "Scenes/GameScene.hpp"
#include "Scenes/GameOverScene.hpp"
#include "Scenes/CampaignLevelScene.hpp"
#include "Scenes/CampaignVictoryScene.hpp"

// ── Global state ───────────────────────────────────────────────────────────
int g_difficulty = 1;  // Quick-play difficulty: 0=Easy 1=Medium 2=Hard

int main()
{
    engine::EngineConfig cfg;
    cfg.title        = "Dungeon of Despair";
    cfg.windowWidth  = 1280;
    cfg.windowHeight = 720;
    cfg.vsync        = true;

    engine::GameEngine game(cfg);

    // ── Legacy / quick-play scenes ─────────────────────────────────────────
    game.scenes().registerScene(engine::SceneID::Menu,
        []() { return std::make_unique<MenuScene>(); });
    game.scenes().registerScene(engine::SceneID::Game,
        []() { return std::make_unique<GameScene>(); });
    game.scenes().registerScene(engine::SceneID::GameOver,
        []() { return std::make_unique<GameOverScene>(); });

    // ── Campaign scenes ────────────────────────────────────────────────────
    // CampaignLevelScene is reused for all 3 levels;
    // CampaignState::current() supplies the per-level config on enter.
    game.scenes().registerScene(engine::SceneID::Level1,
        []() { return std::make_unique<CampaignLevelScene>(); });
    game.scenes().registerScene(engine::SceneID::Level2,
        []() { return std::make_unique<CampaignLevelScene>(); });
    game.scenes().registerScene(engine::SceneID::Level3,
        []() { return std::make_unique<CampaignLevelScene>(); });
    game.scenes().registerScene(engine::SceneID::CampaignVictory,
        []() { return std::make_unique<CampaignVictoryScene>(); });

    game.scenes().switchTo(engine::SceneID::Menu);
    game.run();
    return 0;
}
