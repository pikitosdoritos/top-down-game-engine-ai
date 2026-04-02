#pragma once
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

namespace engine {

// Simple two-layer health bar (background + fill).
// Coordinates are in screen (UI) space.
class HealthBar {
public:
    HealthBar(sf::Vector2f position, sf::Vector2f size,
              sf::Color fillColor    = sf::Color(200, 30, 30),
              sf::Color bgColor      = sf::Color(60, 60, 60),
              sf::Color borderColor  = sf::Color::Black);

    void setRatio(float ratio);  // 0.0–1.0
    void setPosition(sf::Vector2f pos);

    void draw(sf::RenderWindow& window) const;

private:
    sf::RectangleShape m_bg;
    sf::RectangleShape m_fill;
    sf::Vector2f       m_fullSize;
};

} // namespace engine
