#pragma once
#include "engine/Scene/IScene.hpp"
#include "engine/UI/Label.hpp"
#include <SFML/Window/Event.hpp>
#include <optional>

namespace engine { class GameEngine; }

class MenuScene : public engine::IScene {
public:
    void onEnter(engine::GameEngine& engine) override;
    void onExit(engine::GameEngine& engine)  override;
    void handleEvent(engine::GameEngine& engine, const sf::Event& event) override;
    void update(engine::GameEngine& engine, float dt) override;
    void render(engine::GameEngine& engine) override;

private:
    int m_selectedDiff = 1; // 0=Easy, 1=Medium, 2=Hard
    std::optional<engine::Label> m_btnEasy;
    std::optional<engine::Label> m_btnMedium;
    std::optional<engine::Label> m_btnHard;

    std::optional<engine::Label> m_title;
    std::optional<engine::Label> m_subtitle;
    std::optional<engine::Label> m_prompt;
    float m_blinkTimer = 0.f;
    bool  m_promptVisible = true;
};
