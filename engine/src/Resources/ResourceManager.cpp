#include "engine/Resources/ResourceManager.hpp"
#include <stdexcept>

namespace engine {

const sf::Texture& ResourceManager::loadTexture(const std::string& name,
                                                  const std::string& path)
{
    auto it = m_textures.find(name);
    if (it != m_textures.end()) return it->second;

    sf::Texture tex;
    if (!tex.loadFromFile(path))
        throw std::runtime_error("ResourceManager: failed to load texture: " + path);

    m_textures[name] = std::move(tex);
    return m_textures[name];
}

const sf::Texture* ResourceManager::getTexture(const std::string& name) const
{
    auto it = m_textures.find(name);
    return it != m_textures.end() ? &it->second : nullptr;
}

bool ResourceManager::hasTexture(const std::string& name) const
{
    return m_textures.count(name) > 0;
}

const sf::Font& ResourceManager::loadFont(const std::string& name,
                                           const std::string& path)
{
    auto it = m_fonts.find(name);
    if (it != m_fonts.end()) return it->second;

    sf::Font font;
    // SFML 3: Font uses openFromFile instead of loadFromFile
    if (!font.openFromFile(path))
        throw std::runtime_error("ResourceManager: failed to load font: " + path);

    m_fonts[name] = std::move(font);
    return m_fonts[name];
}

const sf::Font* ResourceManager::getFont(const std::string& name) const
{
    auto it = m_fonts.find(name);
    return it != m_fonts.end() ? &it->second : nullptr;
}

bool ResourceManager::hasFont(const std::string& name) const
{
    return m_fonts.count(name) > 0;
}

void ResourceManager::clear()
{
    m_textures.clear();
    m_fonts.clear();
}

} // namespace engine
