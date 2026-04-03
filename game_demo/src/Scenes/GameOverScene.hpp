#pragma once
#include "engine/Scene/IScene.hpp"

// Written by GameScene before transitioning here
extern bool g_playerWon;
extern int  g_killCount;
extern int  g_totalEnemies;

#include "engine/UI/Label.hpp"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Event.hpp>
#include <optional>

namespace engine { class GameEngine; }

class GameOverScene : public engine::IScene {
public:
    void onEnter(engine::GameEngine& engine) override;
    void onExit(engine::GameEngine& engine)  override;
    void handleEvent(engine::GameEngine& engine, const sf::Event& event) override;
    void update(engine::GameEngine& engine, float dt) override;
    void render(engine::GameEngine& engine) override;

private:
    sf::RectangleShape           m_bg;
    sf::RectangleShape           m_ruleTop;
    sf::RectangleShape           m_ruleBot;
    std::optional<engine::Label> m_title;
    std::optional<engine::Label> m_subtitle;
    std::optional<engine::Label> m_stats;
    std::optional<engine::Label> m_prompt;
    std::optional<engine::Label> m_hint;
    float m_blinkTimer    = 0.f;
    bool  m_promptVisible = true;
};
