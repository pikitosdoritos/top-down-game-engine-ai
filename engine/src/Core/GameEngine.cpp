#include "engine/Core/GameEngine.hpp"
#include <SFML/System/Clock.hpp>
#include <algorithm>

namespace engine {

static constexpr float MAX_DELTA = 0.1f; // cap to prevent spiral-of-death

GameEngine::GameEngine(const EngineConfig& cfg)
    : m_config(cfg)
{
    m_window.create(cfg.windowWidth, cfg.windowHeight,
                    cfg.title, cfg.vsync, cfg.fullscreen);
    m_renderer.init(m_window.sfWindow());
}

void GameEngine::run()
{
    m_running = true;
    sf::Clock clock;

    while (m_running && m_window.isOpen()) {
        float dt = std::min(clock.restart().asSeconds(), MAX_DELTA);

        // Apply pending scene switch at start of each frame
        m_scenes.applyPendingSwitch(*this);

        processEvents();
        m_audio.update();
        update(dt);
        render();
    }
}

void GameEngine::quit()
{
    m_running = false;
    m_window.close();
}

void GameEngine::processEvents()
{
    m_input.update(); // snapshot previous frame

    // SFML 3: pollEvent returns std::optional<sf::Event>
    while (const auto event = m_window.sfWindow().pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            quit();
            return;
        }
        m_input.handleEvent(*event);
        m_scenes.handleEvent(*this, *event);
    }
}

void GameEngine::update(float dt)
{
    m_scenes.update(*this, dt);
}

void GameEngine::render()
{
    m_renderer.beginFrame(sf::Color(10, 5, 15));
    m_scenes.render(*this);
    m_renderer.endFrame();
}

} // namespace engine
