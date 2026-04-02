#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/View.hpp>

namespace engine {

class Renderer {
public:
    void init(sf::RenderWindow& window);

    void beginFrame(sf::Color clearColor = sf::Color::Black);
    void endFrame();

    // Expose raw window for anything that needs direct draw access
    sf::RenderWindow* window() { return m_window; }

    // Helpers for switching between world view and UI view
    void setWorldView(const sf::View& view);
    void setUIView();     // sets default (pixel) view for HUD rendering
    void resetView();

private:
    sf::RenderWindow* m_window = nullptr;
};

} // namespace engine
