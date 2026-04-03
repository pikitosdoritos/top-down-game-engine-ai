#include "Scenes/CampaignVictoryScene.hpp"
#include "engine/Core/GameEngine.hpp"
#include "engine/Core/Types.hpp"
#include <SFML/Window/Event.hpp>

void CampaignVictoryScene::onEnter(engine::GameEngine& engine)
{
    auto& res = engine.resources();
    const sf::Font& font = res.loadFont("main", "assets/fonts/Monocraft.ttf");

    float W = static_cast<float>(engine.window().width());
    float H = static_cast<float>(engine.window().height());

    m_title.emplace(font, "CAMPAIGN COMPLETE!", 56, sf::Color(220, 200, 60));
    m_title->centerX(W);
    m_title->setPosition({m_title->sfText()->getPosition().x, H * 0.28f});

    m_sub.emplace(font,
        "You descended into the Abyss and returned victorious.",
        20, sf::Color(180, 160, 100));
    m_sub->centerX(W);
    m_sub->setPosition({m_sub->sfText()->getPosition().x, H * 0.45f});

    m_prompt.emplace(font, "PRESS  ENTER  TO  RETURN  TO  MENU",
                     22, sf::Color(160, 140, 80));
    m_prompt->centerX(W);
    m_prompt->setPosition({m_prompt->sfText()->getPosition().x, H * 0.65f});
}

void CampaignVictoryScene::onExit(engine::GameEngine& /*engine*/)
{
    m_title.reset();
    m_sub.reset();
    m_prompt.reset();
}

void CampaignVictoryScene::handleEvent(engine::GameEngine& engine, const sf::Event& event)
{
    if (const auto* kp = event.getIf<sf::Event::KeyPressed>()) {
        if (kp->code == sf::Keyboard::Key::Enter ||
            kp->code == sf::Keyboard::Key::Space)
        {
            engine.scenes().switchTo(engine::SceneID::Menu);
        }
        if (kp->code == sf::Keyboard::Key::Escape)
            engine.quit();
    }
}

void CampaignVictoryScene::update(engine::GameEngine& /*engine*/, float dt)
{
    m_blinkTimer += dt;
    if (m_blinkTimer >= 0.55f) {
        m_blinkTimer = 0.f;
        m_promptVis  = !m_promptVis;
    }
}

void CampaignVictoryScene::render(engine::GameEngine& engine)
{
    auto* win = engine.renderer().window();
    if (m_title)  m_title->draw(*win);
    if (m_sub)    m_sub->draw(*win);
    if (m_prompt && m_promptVis) m_prompt->draw(*win);
}
