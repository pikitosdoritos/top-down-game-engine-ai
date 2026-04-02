#include "engine/Components/SpriteComponent.hpp"
#include <cmath>

namespace engine {

void SpriteComponent::setTexture(const sf::Texture& tex)
{
    m_texture = &tex;
    // SFML 3: Sprite requires texture at construction
    m_sprite.emplace(tex);
}

void SpriteComponent::setTextureRect(const sf::IntRect& rect)
{
    if (m_sprite)
        m_sprite->setTextureRect(rect);
}

void SpriteComponent::setOriginToCenter()
{
    if (!m_sprite) return;
    auto bounds = m_sprite->getLocalBounds();
    m_sprite->setOrigin({bounds.size.x * 0.5f, bounds.size.y * 0.5f});
}

void SpriteComponent::draw(sf::RenderWindow& window,
                            const sf::Vector2f& pos, float rotation)
{
    if (!m_sprite) return;

    m_sprite->setPosition(pos);
    m_sprite->setRotation(sf::degrees(rotation));

    sf::Vector2f sc = m_sprite->getScale();
    float sx = flipX ? -std::abs(sc.x) : std::abs(sc.x);
    float sy = flipY ? -std::abs(sc.y) : std::abs(sc.y);
    m_sprite->setScale({sx, sy});

    window.draw(*m_sprite);
}

} // namespace engine
