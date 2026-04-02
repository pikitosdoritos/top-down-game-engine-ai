#pragma once
#include <SFML/Window/Event.hpp>

namespace engine {

class GameEngine;

class IScene {
public:
    virtual ~IScene() = default;

    // Called once when the scene becomes active
    virtual void onEnter(GameEngine& engine) = 0;
    // Called once when the scene is about to be replaced
    virtual void onExit(GameEngine& engine)  = 0;

    virtual void handleEvent(engine::GameEngine& engine, const sf::Event& event) {}
    virtual void update(GameEngine& engine, float dt) = 0;
    virtual void render(GameEngine& engine)           = 0;
};

} // namespace engine
