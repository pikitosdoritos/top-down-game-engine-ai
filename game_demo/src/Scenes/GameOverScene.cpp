#include "Scenes/GameOverScene.hpp"
#include "engine/Core/GameEngine.hpp"
#include <SFML/Window/Event.hpp>
#include <string>

// Set by GameScene before switching here
bool g_playerWon    = false;
int  g_killCount    = 0;
int  g_totalEnemies = 0;

void GameOverScene::onEnter(engine::GameEngine& engine)
{
    auto& res = engine.resources();
    const sf::Font& font = res.loadFont("main", "assets/fonts/Monocraft.ttf");

    float W = static_cast<float>(engine.window().width());
    float H = static_cast<float>(engine.window().height());

    // Dark semi-transparent panel behind text
    float panelW = 480.f, panelH = 260.f;
    m_panel.setSize({panelW, panelH});
    m_panel.setOrigin({panelW * 0.5f, panelH * 0.5f});
    m_panel.setPosition({W * 0.5f, H * 0.5f});
    m_panel.setFillColor(sf::Color(10, 5, 20, 210));
    m_panel.setOutlineColor(g_playerWon ? sf::Color(180, 160, 40) : sf::Color(160, 30, 30));
    m_panel.setOutlineThickness(2.f);

    // Title
    if (g_playerWon) {
        m_title.emplace(font, "VICTORY", 64, sf::Color(220, 200, 60));
    } else {
        m_title.emplace(font, "YOU  DIED", 64, sf::Color(200, 40, 40));
    }
    m_title->centerX(W);
    m_title->setPosition({m_title->sfText()->getPosition().x, H * 0.30f});

    // Stats line
    std::string statsStr = "Enemies slain:  " + std::to_string(g_killCount)
                         + " / " + std::to_string(g_totalEnemies);
    m_stats.emplace(font, statsStr, 22, sf::Color(160, 200, 160));
    m_stats->centerX(W);
    m_stats->setPosition({m_stats->sfText()->getPosition().x, H * 0.50f});

    // Prompt
    m_prompt.emplace(font, "PRESS  ENTER  TO  RETURN", 22, sf::Color(180, 160, 100));
    m_prompt->centerX(W);
    m_prompt->setPosition({m_prompt->sfText()->getPosition().x, H * 0.62f});
}

void GameOverScene::onExit(engine::GameEngine& /*engine*/)
{
    m_title.reset();
    m_stats.reset();
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
    win->draw(m_panel);
    if (m_title)  m_title->draw(*win);
    if (m_stats)  m_stats->draw(*win);
    if (m_prompt && m_promptVisible) m_prompt->draw(*win);
}
