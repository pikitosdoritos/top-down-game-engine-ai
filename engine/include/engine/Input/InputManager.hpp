#pragma once
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <array>

namespace engine {

class InputManager {
public:
    // Call once per frame before update (snapshots previous state)
    void update();
    // Feed each SFML event from the poll loop
    void handleEvent(const sf::Event& event);

    // Key state queries — use sf::Keyboard::Key enum class values
    bool isKeyDown(sf::Keyboard::Key key)     const;
    bool isKeyPressed(sf::Keyboard::Key key)  const; // true only on first frame down
    bool isKeyReleased(sf::Keyboard::Key key) const; // true only on release frame

    bool isMouseDown(sf::Mouse::Button btn)     const;
    bool isMousePressed(sf::Mouse::Button btn)  const;
    bool isMouseReleased(sf::Mouse::Button btn) const;

    // Mouse position in window pixel coords
    sf::Vector2i mousePosition(const sf::RenderWindow& win) const;
    // Mouse position mapped through the window's current view (world coords)
    sf::Vector2f mouseWorldPosition(const sf::RenderWindow& win) const;

private:
    static constexpr int KEY_COUNT = static_cast<int>(sf::Keyboard::KeyCount);
    static constexpr int BTN_COUNT = static_cast<int>(sf::Mouse::ButtonCount);

    std::array<bool, KEY_COUNT> m_keyCurrent  {};
    std::array<bool, KEY_COUNT> m_keyPrevious {};

    std::array<bool, BTN_COUNT> m_btnCurrent  {};
    std::array<bool, BTN_COUNT> m_btnPrevious {};
};

} // namespace engine
