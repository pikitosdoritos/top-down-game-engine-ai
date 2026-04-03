#pragma once
#include "engine/Scene/IScene.hpp"
#include "engine/UI/Label.hpp"
#include <SFML/Window/Event.hpp>
#include <optional>

namespace engine { class GameEngine; }

class CampaignVictoryScene : public engine::IScene {
public:
    void onEnter(engine::GameEngine& engine)  override;
    void onExit(engine::GameEngine& engine)   override;
    void handleEvent(engine::GameEngine& engine, const sf::Event& event) override;
    void update(engine::GameEngine& engine, float dt) override;
    void render(engine::GameEngine& engine)   override;

private:
    std::optional<engine::Label> m_title;
    std::optional<engine::Label> m_sub;
    std::optional<engine::Label> m_prompt;
    float m_blinkTimer   = 0.f;
    bool  m_promptVis    = true;
};
