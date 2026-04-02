#include "engine/Input/InputManager.hpp"

namespace engine {

void InputManager::update()
{
    m_keyPrevious = m_keyCurrent;
    m_btnPrevious = m_btnCurrent;
}

void InputManager::handleEvent(const sf::Event& event)
{
    // SFML 3: events are variant-based — use getIf<T>()
    if (const auto* kp = event.getIf<sf::Event::KeyPressed>()) {
        int idx = static_cast<int>(kp->code);
        if (idx >= 0 && idx < KEY_COUNT)
            m_keyCurrent[idx] = true;
    }
    else if (const auto* kr = event.getIf<sf::Event::KeyReleased>()) {
        int idx = static_cast<int>(kr->code);
        if (idx >= 0 && idx < KEY_COUNT)
            m_keyCurrent[idx] = false;
    }
    else if (const auto* mb = event.getIf<sf::Event::MouseButtonPressed>()) {
        int idx = static_cast<int>(mb->button);
        if (idx >= 0 && idx < BTN_COUNT)
            m_btnCurrent[idx] = true;
    }
    else if (const auto* mr = event.getIf<sf::Event::MouseButtonReleased>()) {
        int idx = static_cast<int>(mr->button);
        if (idx >= 0 && idx < BTN_COUNT)
            m_btnCurrent[idx] = false;
    }
}

bool InputManager::isKeyDown(sf::Keyboard::Key key) const
{
    int idx = static_cast<int>(key);
    return idx >= 0 && idx < KEY_COUNT && m_keyCurrent[idx];
}

bool InputManager::isKeyPressed(sf::Keyboard::Key key) const
{
    int idx = static_cast<int>(key);
    return idx >= 0 && idx < KEY_COUNT &&
           m_keyCurrent[idx] && !m_keyPrevious[idx];
}

bool InputManager::isKeyReleased(sf::Keyboard::Key key) const
{
    int idx = static_cast<int>(key);
    return idx >= 0 && idx < KEY_COUNT &&
           !m_keyCurrent[idx] && m_keyPrevious[idx];
}

bool InputManager::isMouseDown(sf::Mouse::Button btn) const
{
    int idx = static_cast<int>(btn);
    return idx >= 0 && idx < BTN_COUNT && m_btnCurrent[idx];
}

bool InputManager::isMousePressed(sf::Mouse::Button btn) const
{
    int idx = static_cast<int>(btn);
    return idx >= 0 && idx < BTN_COUNT &&
           m_btnCurrent[idx] && !m_btnPrevious[idx];
}

bool InputManager::isMouseReleased(sf::Mouse::Button btn) const
{
    int idx = static_cast<int>(btn);
    return idx >= 0 && idx < BTN_COUNT &&
           !m_btnCurrent[idx] && m_btnPrevious[idx];
}

sf::Vector2i InputManager::mousePosition(const sf::RenderWindow& win) const
{
    return sf::Mouse::getPosition(win);
}

sf::Vector2f InputManager::mouseWorldPosition(const sf::RenderWindow& win) const
{
    return win.mapPixelToCoords(sf::Mouse::getPosition(win));
}

sf::Vector2f InputManager::mouseWorldPosition(const sf::RenderWindow& win, const sf::View& view) const
{
    return win.mapPixelToCoords(sf::Mouse::getPosition(win), view);
}

} // namespace engine
