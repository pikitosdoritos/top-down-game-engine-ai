#include "Scenes/MenuScene.hpp"
#include "engine/Core/GameEngine.hpp"
#include "engine/Core/Types.hpp"
#include <SFML/Window/Event.hpp>

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
    m_prompt->setPosition({m_prompt->sfText()->getPosition().x, H * 0.62f});
}

void MenuScene::onExit(engine::GameEngine& /*engine*/)
{
    m_title.reset();
    m_subtitle.reset();
    m_prompt.reset();
}

void MenuScene::handleEvent(engine::GameEngine& engine, const sf::Event& event)
{
    if (const auto* kp = event.getIf<sf::Event::KeyPressed>()) {
        if (kp->code == sf::Keyboard::Key::Enter ||
            kp->code == sf::Keyboard::Key::Space)
        {
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
}

void MenuScene::render(engine::GameEngine& engine)
{
    auto* win = engine.renderer().window();

    // Dark vignette background is provided by the engine clear colour.
    if (m_title)    m_title->draw(*win);
    if (m_subtitle) m_subtitle->draw(*win);
    if (m_prompt && m_promptVisible) m_prompt->draw(*win);
}
