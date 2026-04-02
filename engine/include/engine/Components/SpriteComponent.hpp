#pragma once
#include "engine/Entity/Component.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <optional>

namespace engine {

// SFML 3: sf::Sprite requires a texture at construction — we hold it as optional.
class SpriteComponent : public Component {
public:
    SpriteComponent() = default;

    // Set (or replace) the texture. Creates/recreates the internal sprite.
    void setTexture(const sf::Texture& tex);
    void setTextureRect(const sf::IntRect& rect);
    void setOriginToCenter();

    // Returns nullptr if no texture has been set yet
    sf::Sprite*       sprite()       { return m_sprite ? &*m_sprite : nullptr; }
    const sf::Sprite* sprite() const { return m_sprite ? &*m_sprite : nullptr; }

    bool isReady() const { return m_sprite.has_value(); }

    // Draw at given world position
    void draw(sf::RenderWindow& window, const sf::Vector2f& pos, float rotation = 0.f);

    int  zOrder = 0;
    bool flipX  = false;
    bool flipY  = false;

private:
    std::optional<sf::Sprite>  m_sprite;
    const sf::Texture*         m_texture = nullptr;
};

} // namespace engine
