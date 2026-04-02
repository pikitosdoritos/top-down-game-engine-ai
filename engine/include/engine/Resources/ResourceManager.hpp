#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <string>
#include <unordered_map>
#include <stdexcept>

namespace engine {

// Typed asset cache. Loads on first request, returns same instance thereafter.
// Assets stay alive as long as the ResourceManager exists.
class ResourceManager {
public:
    // Textures
    const sf::Texture& loadTexture(const std::string& name, const std::string& path);
    const sf::Texture* getTexture(const std::string& name) const;
    bool               hasTexture(const std::string& name) const;

    // Fonts
    const sf::Font& loadFont(const std::string& name, const std::string& path);
    const sf::Font* getFont(const std::string& name) const;
    bool            hasFont(const std::string& name) const;

    void clear(); // release all cached assets

private:
    std::unordered_map<std::string, sf::Texture> m_textures;
    std::unordered_map<std::string, sf::Font>    m_fonts;
};

} // namespace engine
