#pragma once
#include "engine/Core/Types.hpp"
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

namespace engine {

class Camera {
public:
    Camera() = default;
    explicit Camera(sf::Vector2f viewSize);

    void follow(Vec2f target, float dt, float lerpSpeed = 5.f);
    void setCenter(Vec2f pos);
    void setBounds(FloatRect worldBounds);
    void apply(sf::RenderWindow& window) const;

    const sf::View& sfView() const { return m_view; }
    sf::View&       sfView()       { return m_view; }

    void setViewSize(sf::Vector2f size) { m_view.setSize(size); }

private:
    sf::View  m_view;
    // SFML 3 FloatRect: default init with two Vector2f
    FloatRect m_bounds      { {0.f, 0.f}, {0.f, 0.f} };
    bool      m_hasBounds   = false;

    void clampToBounds();
};

} // namespace engine
