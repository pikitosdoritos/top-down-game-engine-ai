#pragma once

namespace engine {

class Entity;
class GameEngine;

class Component {
public:
    virtual ~Component() = default;

    // Optional lifecycle hooks
    virtual void onAttach(Entity& owner) {}
    virtual void update(Entity& owner, GameEngine& engine, float dt) {}

    bool enabled = true;

protected:
    Component() = default;
};

} // namespace engine
