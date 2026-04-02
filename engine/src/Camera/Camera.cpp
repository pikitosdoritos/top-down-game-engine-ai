#include "engine/Camera/Camera.hpp"
#include <algorithm>
#include <cmath>

namespace engine {

Camera::Camera(sf::Vector2f viewSize)
{
    m_view.setSize(viewSize);
}

void Camera::follow(Vec2f target, float dt, float lerpSpeed)
{
    sf::Vector2f current = m_view.getCenter();
    float t = 1.f - std::pow(1.f - std::min(lerpSpeed * dt, 1.f), 1.f);
    m_view.setCenter(
        {current.x + (target.x - current.x) * t,
         current.y + (target.y - current.y) * t}
    );
    clampToBounds();
}

void Camera::setCenter(Vec2f pos)
{
    m_view.setCenter({pos.x, pos.y});
    clampToBounds();
}

void Camera::setBounds(FloatRect worldBounds)
{
    m_bounds    = worldBounds;
    m_hasBounds = true;
}

void Camera::apply(sf::RenderWindow& window) const
{
    window.setView(m_view);
}

void Camera::clampToBounds()
{
    if (!m_hasBounds) return;

    // SFML 3: FloatRect uses .position and .size
    sf::Vector2f half   = m_view.getSize() * 0.5f;
    sf::Vector2f center = m_view.getCenter();

    float minX = m_bounds.position.x + half.x;
    float maxX = m_bounds.position.x + m_bounds.size.x - half.x;
    float minY = m_bounds.position.y + half.y;
    float maxY = m_bounds.position.y + m_bounds.size.y - half.y;

    if (maxX < minX) { minX = maxX = m_bounds.position.x + m_bounds.size.x * 0.5f; }
    if (maxY < minY) { minY = maxY = m_bounds.position.y + m_bounds.size.y * 0.5f; }

    center.x = std::max(minX, std::min(center.x, maxX));
    center.y = std::max(minY, std::min(center.y, maxY));
    m_view.setCenter(center);
}

} // namespace engine
