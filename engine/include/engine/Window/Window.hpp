#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <string>

namespace engine {

class Window {
public:
    Window() = default;
    void create(unsigned width, unsigned height,
                const std::string& title, bool vsync, bool fullscreen);

    sf::RenderWindow& sfWindow()       { return m_window; }
    const sf::RenderWindow& sfWindow() const { return m_window; }

    bool isOpen() const { return m_window.isOpen(); }
    void close()        { m_window.close(); }

    unsigned width()  const { return m_window.getSize().x; }
    unsigned height() const { return m_window.getSize().y; }

private:
    sf::RenderWindow m_window;
};

} // namespace engine
