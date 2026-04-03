#include "Scenes/MenuScene.hpp"
#include "Campaign/CampaignState.hpp"
#include "engine/Core/GameEngine.hpp"
#include "engine/Core/Types.hpp"
#include <SFML/Window/Event.hpp>

extern int g_difficulty;

using namespace engine;

void MenuScene::onEnter(engine::GameEngine& engine)
{
    auto& res = engine.resources();
    const sf::Font& font = res.loadFont("main", "assets/fonts/Monocraft.ttf");

    float W = static_cast<float>(engine.window().width());
    float H = static_cast<float>(engine.window().height());

    m_title.emplace(font, "DUNGEON OF DESPAIR", 52, sf::Color(200, 60, 60));
    m_title->centerX(W);
    m_title->setPosition({m_title->sfText()->getPosition().x, H * 0.18f});

    m_subtitle.emplace(font, "A Diablo-Like Demo", 20, sf::Color(160, 110, 60));
    m_subtitle->centerX(W);
    m_subtitle->setPosition({m_subtitle->sfText()->getPosition().x, H * 0.30f});

    // ── Mode buttons ──────────────────────────────────────────────────────
    m_btnCampaign.emplace(font, "[ CAMPAIGN  MODE ]", 30, sf::Color::White);
    m_btnCampaign->centerX(W);
    m_btnCampaign->setPosition({m_btnCampaign->sfText()->getPosition().x, H * 0.43f});

    m_btnQuickPlay.emplace(font, "[ QUICK  PLAY ]", 30, sf::Color(100, 100, 100));
    m_btnQuickPlay->centerX(W);
    m_btnQuickPlay->setPosition({m_btnQuickPlay->sfText()->getPosition().x, H * 0.53f});

    // ── Quick-play difficulty ─────────────────────────────────────────────
    m_btnEasy.emplace(font, "EASY", 22, sf::Color(100, 100, 100));
    m_btnEasy->centerX(W);
    m_btnEasy->setPosition({m_btnEasy->sfText()->getPosition().x, H * 0.64f});

    m_btnMedium.emplace(font, "MEDIUM", 22, sf::Color(100, 100, 100));
    m_btnMedium->centerX(W);
    m_btnMedium->setPosition({m_btnMedium->sfText()->getPosition().x, H * 0.71f});

    m_btnHard.emplace(font, "HARD", 22, sf::Color(100, 100, 100));
    m_btnHard->centerX(W);
    m_btnHard->setPosition({m_btnHard->sfText()->getPosition().x, H * 0.78f});

    m_controls.emplace(font, "W/S: Navigate    Enter: Select    F11: Fullscreen    Esc: Quit", 14,
                        sf::Color(80, 70, 60));
    m_controls->centerX(W);
    m_controls->setPosition({m_controls->sfText()->getPosition().x, H * 0.92f});

    m_prompt.emplace(font, "PRESS  ENTER  TO  BEGIN", 22, sf::Color(200, 180, 100));
    m_prompt->centerX(W);
    m_prompt->setPosition({m_prompt->sfText()->getPosition().x, H * 0.86f});
}

void MenuScene::onExit(engine::GameEngine& /*engine*/)
{
    m_title.reset();
    m_subtitle.reset();
    m_btnCampaign.reset();
    m_btnQuickPlay.reset();
    m_btnEasy.reset();
    m_btnMedium.reset();
    m_btnHard.reset();
    m_controls.reset();
    m_prompt.reset();
}

void MenuScene::handleEvent(engine::GameEngine& engine, const sf::Event& event)
{
    if (const auto* kp = event.getIf<sf::Event::KeyPressed>()) {
        // Navigate between Campaign / QuickPlay
        if (kp->code == sf::Keyboard::Key::Up || kp->code == sf::Keyboard::Key::W) {
            if (m_selectedMode == 1 && m_selectedDiff == 0)
                m_selectedMode = 0;                       // up from Easy → QuickPlay → Campaign
            else if (m_selectedMode == 1)
                m_selectedDiff = (m_selectedDiff - 1 + 3) % 3;
            else
                m_selectedMode = 0; // already at top
        }
        if (kp->code == sf::Keyboard::Key::Down || kp->code == sf::Keyboard::Key::S) {
            if (m_selectedMode == 0) {
                m_selectedMode = 1;  // Campaign → QuickPlay
            } else {
                m_selectedDiff = (m_selectedDiff + 1) % 3;
            }
        }

        if (kp->code == sf::Keyboard::Key::Enter ||
            kp->code == sf::Keyboard::Key::Space)
        {
            if (m_selectedMode == 0) {
                // Campaign: reset to level 1 and launch Level1 scene
                CampaignState::get().currentLevel = 1;
                engine.scenes().switchTo(SceneID::Level1);
            } else {
                // Quick play
                g_difficulty = m_selectedDiff;
                engine.scenes().switchTo(SceneID::Game);
            }
        }
        if (kp->code == sf::Keyboard::Key::Escape)
            engine.quit();
    }
}

void MenuScene::update(engine::GameEngine& /*engine*/, float dt)
{
    m_blinkTimer += dt;
    if (m_blinkTimer >= 0.55f) {
        m_blinkTimer    = 0.f;
        m_promptVisible = !m_promptVisible;
    }

    // Highlight selected mode
    if (m_btnCampaign)
        m_btnCampaign->sfText()->setFillColor(
            m_selectedMode == 0 ? sf::Color(220, 200, 60) : sf::Color(100, 100, 100));
    if (m_btnQuickPlay)
        m_btnQuickPlay->sfText()->setFillColor(
            m_selectedMode == 1 ? sf::Color(220, 200, 60) : sf::Color(100, 100, 100));

    // Highlight selected difficulty (only visible when QuickPlay is focused)
    sf::Color inactive{80, 80, 80};
    if (m_btnEasy)
        m_btnEasy->sfText()->setFillColor(
            (m_selectedMode == 1 && m_selectedDiff == 0) ? sf::Color::White : inactive);
    if (m_btnMedium)
        m_btnMedium->sfText()->setFillColor(
            (m_selectedMode == 1 && m_selectedDiff == 1) ? sf::Color(240, 200, 50) : inactive);
    if (m_btnHard)
        m_btnHard->sfText()->setFillColor(
            (m_selectedMode == 1 && m_selectedDiff == 2) ? sf::Color(220, 50, 50) : inactive);
}

void MenuScene::render(engine::GameEngine& engine)
{
    auto* win = engine.renderer().window();

    if (m_title)      m_title->draw(*win);
    if (m_subtitle)   m_subtitle->draw(*win);
    if (m_btnCampaign) m_btnCampaign->draw(*win);
    if (m_btnQuickPlay) m_btnQuickPlay->draw(*win);

    // Only show difficulty when QuickPlay highlighted
    if (m_selectedMode == 1) {
        if (m_btnEasy)   m_btnEasy->draw(*win);
        if (m_btnMedium) m_btnMedium->draw(*win);
        if (m_btnHard)   m_btnHard->draw(*win);
    }

    if (m_controls) m_controls->draw(*win);
    if (m_prompt && m_promptVisible) m_prompt->draw(*win);
}
