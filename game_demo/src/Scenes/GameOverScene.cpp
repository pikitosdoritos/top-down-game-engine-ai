#include "Scenes/GameOverScene.hpp"
#include "engine/Core/GameEngine.hpp"
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
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

    // Full-screen dark overlay panel
    m_bg.setSize({W, H});
    m_bg.setFillColor(sf::Color(8, 4, 12));

    // Decorative horizontal rule lines
    m_ruleTop.setSize({W * 0.55f, 2.f});
    m_ruleTop.setFillColor(g_playerWon ? sf::Color(180, 155, 40, 200) : sf::Color(140, 25, 25, 200));
    m_ruleTop.setOrigin({m_ruleTop.getSize().x * 0.5f, 1.f});
    m_ruleTop.setPosition({W * 0.5f, H * 0.42f});

    m_ruleBot.setSize({W * 0.55f, 2.f});
    m_ruleBot.setFillColor(m_ruleTop.getFillColor());
    m_ruleBot.setOrigin({m_ruleBot.getSize().x * 0.5f, 1.f});
    m_ruleBot.setPosition({W * 0.5f, H * 0.68f});

    // Title — "YOU DIED" or "VICTORY"
    if (g_playerWon) {
        m_title.emplace(font, "VICTORY", 60, sf::Color(220, 195, 55));
    } else {
        m_title.emplace(font, "YOU  DIED", 60, sf::Color(190, 30, 30));
    }
    m_title->centerX(W);
    m_title->setPosition({m_title->sfText()->getPosition().x, H * 0.22f});

    // Subtitle flavour text
    const char* flavour = g_playerWon
        ? "The dungeon falls silent."
        : "Darkness claims another soul.";
    m_subtitle.emplace(font, flavour, 18, sf::Color(120, 100, 80));
    m_subtitle->centerX(W);
    m_subtitle->setPosition({m_subtitle->sfText()->getPosition().x, H * 0.355f});

    // Stats
    std::string statsStr = "Enemies slain:   "
        + std::to_string(g_killCount) + " / " + std::to_string(g_totalEnemies);
    m_stats.emplace(font, statsStr, 22,
        g_playerWon ? sf::Color(160, 210, 160) : sf::Color(160, 100, 100));
    m_stats->centerX(W);
    m_stats->setPosition({m_stats->sfText()->getPosition().x, H * 0.50f});

    // Navigation hint
    m_prompt.emplace(font, "PRESS  ENTER  TO  RETURN", 20, sf::Color(180, 155, 90));
    m_prompt->centerX(W);
    m_prompt->setPosition({m_prompt->sfText()->getPosition().x, H * 0.76f});

    m_hint.emplace(font, "ESC  to  quit", 14, sf::Color(70, 60, 50));
    m_hint->centerX(W);
    m_hint->setPosition({m_hint->sfText()->getPosition().x, H * 0.84f});
}

void GameOverScene::onExit(engine::GameEngine& /*engine*/)
{
    m_title.reset();
    m_subtitle.reset();
    m_stats.reset();
    m_prompt.reset();
    m_hint.reset();
}

void GameOverScene::handleEvent(engine::GameEngine& engine, const sf::Event& event)
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

    // Ensure we're in screen-space (world camera may still be active)
    engine.renderer().setUIView();

    win->draw(m_bg);
    win->draw(m_ruleTop);
    win->draw(m_ruleBot);

    if (m_title)    m_title->draw(*win);
    if (m_subtitle) m_subtitle->draw(*win);
    if (m_stats)    m_stats->draw(*win);
    if (m_prompt && m_promptVisible) m_prompt->draw(*win);
    if (m_hint)     m_hint->draw(*win);
}
