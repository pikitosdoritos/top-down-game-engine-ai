#include "Scenes/GameOverScene.hpp"
#include "engine/Core/GameEngine.hpp"
#include <SFML/Window/Event.hpp>

// Set by GameScene before switching here
bool g_playerWon = false;

void GameOverScene::onEnter(engine::GameEngine& engine)
{
    auto& res = engine.resources();
    const sf::Font& font = res.loadFont("main", "assets/fonts/Monocraft.ttf");

    float W = static_cast<float>(engine.window().width());
    float H = static_cast<float>(engine.window().height());

    if (g_playerWon) {
        m_title.emplace(font, "VICTORY", 64, sf::Color(220, 200, 60));
    } else {
        m_title.emplace(font, "YOU  DIED", 64, sf::Color(200, 40, 40));
    }
    m_title->centerX(W);
    m_title->setPosition({m_title->sfText()->getPosition().x, H * 0.30f});

    m_prompt.emplace(font, "PRESS  ENTER  TO  RETURN", 24, sf::Color(180, 160, 100));
    m_prompt->centerX(W);
    m_prompt->setPosition({m_prompt->sfText()->getPosition().x, H * 0.60f});
}

void GameOverScene::onExit(engine::GameEngine& /*engine*/)
{
    m_title.reset();
    m_prompt.reset();
}

void GameOverScene::handleEvent(engine::GameEngine& engine, const sf::Event& event)
{
    if (const auto* kp = event.getIf<sf::Event::KeyPressed>()) {
        if (kp->code == sf::Keyboard::Key::Enter ||
            kp->code == sf::Keyboard::Key::Space)
        {
            engine.scenes().switchTo(engine::SceneID::Menu);
        }
        if (kp->code == sf::Keyboard::Key::Escape) {
            engine.quit();
        }
    }
}

void GameOverScene::update(engine::GameEngine& /*engine*/, float dt)
{
    m_blinkTimer += dt;
    if (m_blinkTimer >= 0.6f) {
        m_blinkTimer    = 0.f;
        m_promptVisible = !m_promptVisible;
    }
}

void GameOverScene::render(engine::GameEngine& engine)
{
    auto* win = engine.renderer().window();
    if (m_title)  m_title->draw(*win);
    if (m_prompt && m_promptVisible) m_prompt->draw(*win);
}
