#include "engine/Engine.hpp"
#include "Scenes/MenuScene.hpp"
#include "Scenes/GameScene.hpp"
#include "Scenes/GameOverScene.hpp"

int main()
{
    engine::EngineConfig cfg;
    cfg.title        = "Dungeon of Despair";
    cfg.windowWidth  = 1280;
    cfg.windowHeight = 720;
    cfg.vsync        = true;

    engine::GameEngine game(cfg);

    // Register all scenes
    game.scenes().registerScene(engine::SceneID::Menu,
        []() { return std::make_unique<MenuScene>(); });
    game.scenes().registerScene(engine::SceneID::Game,
        []() { return std::make_unique<GameScene>(); });
    game.scenes().registerScene(engine::SceneID::GameOver,
        []() { return std::make_unique<GameOverScene>(); });

    // Start on the menu
    game.scenes().switchTo(engine::SceneID::Menu);

    game.run();
    return 0;
}
