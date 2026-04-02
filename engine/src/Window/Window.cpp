#include "engine/Window/Window.hpp"
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/WindowEnums.hpp>

namespace engine {

void Window::create(unsigned width, unsigned height,
                    const std::string& title, bool vsync, bool fullscreen)
{
    if (fullscreen) {
        // SFML 3: fullscreen uses sf::State::Fullscreen
        m_window.create(sf::VideoMode({width, height}), title, sf::State::Fullscreen);
    } else {
        m_window.create(sf::VideoMode({width, height}), title,
                        sf::Style::Titlebar | sf::Style::Close);
    }
    m_window.setVerticalSyncEnabled(vsync);
    if (!vsync)
        m_window.setFramerateLimit(60);
}

} // namespace engine
