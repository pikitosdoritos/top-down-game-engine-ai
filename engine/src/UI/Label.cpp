#include "engine/UI/Label.hpp"
#include <cmath>

namespace engine {

Label::Label(const sf::Font& font, const std::string& text,
             unsigned charSize, sf::Color color)
{
    // SFML 3: sf::Text(font, string, charSize)
    m_text.emplace(font, text, charSize);
    m_text->setFillColor(color);
}

void Label::init(const sf::Font& font, const std::string& text,
                 unsigned charSize, sf::Color color)
{
    m_text.emplace(font, text, charSize);
    m_text->setFillColor(color);
}

void Label::setString(const std::string& text)
{
    if (m_text) m_text->setString(text);
}

void Label::setPosition(sf::Vector2f pos)
{
    if (m_text) m_text->setPosition(pos);
}

void Label::setColor(sf::Color c)
{
    if (m_text) m_text->setFillColor(c);
}

void Label::setCharSize(unsigned size)
{
    if (m_text) m_text->setCharacterSize(size);
}

void Label::centerX(float windowWidth)
{
    if (!m_text) return;
    float w = m_text->getLocalBounds().size.x;
    m_text->setPosition({std::floor((windowWidth - w) * 0.5f),
                          m_text->getPosition().y});
}

void Label::draw(sf::RenderWindow& window) const
{
    if (m_text) window.draw(*m_text);
}

} // namespace engine
