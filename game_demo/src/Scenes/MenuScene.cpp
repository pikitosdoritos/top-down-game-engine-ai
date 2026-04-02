#include "Scenes/MenuScene.hpp"
#include "engine/Core/GameEngine.hpp"
#include "engine/Core/Types.hpp"
#include <SFML/Window/Event.hpp>

extern int g_difficulty;

void MenuScene::onEnter(engine::GameEngine& engine)
{
    auto& res = engine.resources();
    const sf::Font& font = res.loadFont("main", "assets/fonts/Monocraft.ttf");

    float W = static_cast<float>(engine.window().width());
    float H = static_cast<float>(engine.window().height());

    m_title.emplace(font, "DUNGEON OF DESPAIR", 52, sf::Color(200, 60, 60));
    m_title->centerX(W);
    m_title->setPosition({m_title->sfText()->getPosition().x, H * 0.28f});

    m_subtitle.emplace(font, "A Diablo-Like Demo", 22, sf::Color(160, 120, 80));
    m_subtitle->centerX(W);
    m_subtitle->setPosition({m_subtitle->sfText()->getPosition().x, H * 0.42f});

    m_prompt.emplace(font, "PRESS  ENTER  TO  BEGIN", 26, sf::Color(200, 180, 100));
    m_prompt->centerX(W);
    m_prompt->setPosition({m_prompt->sfText()->getPosition().x, H * 0.82f});

    m_btnEasy.emplace(font, "EASY", 24, sf::Color(100, 100, 100));
    m_btnEasy->centerX(W);
    m_btnEasy->setPosition({m_btnEasy->sfText()->getPosition().x, H * 0.55f});

    m_btnMedium.emplace(font, "MEDIUM", 24, sf::Color(100, 100, 100));
    m_btnMedium->centerX(W);
    m_btnMedium->setPosition({m_btnMedium->sfText()->getPosition().x, H * 0.62f});

    m_btnHard.emplace(font, "HARD", 24, sf::Color(100, 100, 100));
    m_btnHard->centerX(W);
    m_btnHard->setPosition({m_btnHard->sfText()->getPosition().x, H * 0.69f});
}

void MenuScene::onExit(engine::GameEngine& /*engine*/)
{
    m_title.reset();
    m_subtitle.reset();
    m_btnEasy.reset();
    m_btnMedium.reset();
    m_btnHard.reset();
    m_prompt.reset();
}

void MenuScene::handleEvent(engine::GameEngine& engine, const sf::Event& event)
{
    if (const auto* kp = event.getIf<sf::Event::KeyPressed>()) {
        if (kp->code == sf::Keyboard::Key::Up || kp->code == sf::Keyboard::Key::W) {
            m_selectedDiff = (m_selectedDiff - 1 + 3) % 3;
        }
        if (kp->code == sf::Keyboard::Key::Down || kp->code == sf::Keyboard::Key::S) {
            m_selectedDiff = (m_selectedDiff + 1) % 3;
        }
        if (kp->code == sf::Keyboard::Key::Enter ||
            kp->code == sf::Keyboard::Key::Space)
        {
            g_difficulty = m_selectedDiff;
            engine.scenes().switchTo(engine::SceneID::Game);
        }
        if (kp->code == sf::Keyboard::Key::Escape) {
            engine.quit();
        }
    }
}

void MenuScene::update(engine::GameEngine& /*engine*/, float dt)
{
    m_blinkTimer += dt;
    if (m_blinkTimer >= 0.55f) {
        m_blinkTimer     = 0.f;
        m_promptVisible  = !m_promptVisible;
    }

    if (m_btnEasy) m_btnEasy->sfText()->setFillColor(m_selectedDiff == 0 ? sf::Color::White : sf::Color(100, 100, 100));
    if (m_btnMedium) m_btnMedium->sfText()->setFillColor(m_selectedDiff == 1 ? sf::Color(240, 200, 50) : sf::Color(100, 100, 100));
    if (m_btnHard) m_btnHard->sfText()->setFillColor(m_selectedDiff == 2 ? sf::Color(220, 50, 50) : sf::Color(100, 100, 100));
}

void MenuScene::render(engine::GameEngine& engine)
{
    auto* win = engine.renderer().window();

    // Dark vignette background is provided by the engine clear colour.
    if (m_title)    m_title->draw(*win);
    if (m_subtitle) m_subtitle->draw(*win);
    if (m_btnEasy)  m_btnEasy->draw(*win);
    if (m_btnMedium) m_btnMedium->draw(*win);
    if (m_btnHard)   m_btnHard->draw(*win);
    if (m_prompt && m_promptVisible) m_prompt->draw(*win);
}
