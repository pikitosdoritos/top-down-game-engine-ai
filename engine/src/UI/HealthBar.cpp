#include "engine/UI/HealthBar.hpp"

namespace engine {

HealthBar::HealthBar(sf::Vector2f position, sf::Vector2f size,
                     sf::Color fillColor, sf::Color bgColor, sf::Color borderColor)
    : m_fullSize(size)
{
    m_bg.setSize(size);
    m_bg.setFillColor(bgColor);
    m_bg.setOutlineColor(borderColor);
    m_bg.setOutlineThickness(1.f);
    m_bg.setPosition(position);

    m_fill.setSize(size);
    m_fill.setFillColor(fillColor);
    m_fill.setPosition(position);
}

void HealthBar::setRatio(float ratio)
{
    ratio = std::max(0.f, std::min(1.f, ratio));
    m_fill.setSize({m_fullSize.x * ratio, m_fullSize.y});
}

void HealthBar::setPosition(sf::Vector2f pos)
{
    m_bg.setPosition(pos);
    m_fill.setPosition(pos);
}

void HealthBar::draw(sf::RenderWindow& window) const
{
    window.draw(m_bg);
    window.draw(m_fill);
}

} // namespace engine
