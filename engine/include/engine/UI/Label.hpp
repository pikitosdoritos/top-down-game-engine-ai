#pragma once
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <optional>
#include <string>

namespace engine {

// SFML 3: sf::Text requires a font at construction, so we hold it as optional.
class Label {
public:
    Label() = default;
    Label(const sf::Font& font, const std::string& text,
          unsigned charSize = 24, sf::Color color = sf::Color::White);

    // Initialise after default construction
    void init(const sf::Font& font, const std::string& text,
              unsigned charSize = 24, sf::Color color = sf::Color::White);

    void setString(const std::string& text);
    void setPosition(sf::Vector2f pos);
    void setColor(sf::Color c);
    void setCharSize(unsigned size);
    void centerX(float windowWidth);

    void draw(sf::RenderWindow& window) const;

    sf::Text*       sfText()       { return m_text ? &*m_text : nullptr; }
    const sf::Text* sfText() const { return m_text ? &*m_text : nullptr; }
    bool isReady() const { return m_text.has_value(); }

private:
    std::optional<sf::Text> m_text;
};

} // namespace engine
