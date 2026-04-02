#pragma once
#include "engine/Scene/IScene.hpp"
#include "engine/Core/Types.hpp"
#include <SFML/Window/Event.hpp>
#include <memory>
#include <functional>
#include <unordered_map>

namespace engine {

class GameEngine;

class SceneManager {
public:
    // Register a factory for a scene ID
    void registerScene(SceneID id, std::function<std::unique_ptr<IScene>()> factory);

    // Request transition to scene (applied at start of next frame)
    void switchTo(SceneID id);

    void handleEvent(GameEngine& engine, const sf::Event& event);
    void update(GameEngine& engine, float dt);
    void render(GameEngine& engine);

    // Called by GameEngine at the top of each frame to apply pending switch
    void applyPendingSwitch(GameEngine& engine);

    IScene* current() { return m_current.get(); }

private:
    std::unordered_map<SceneID,
        std::function<std::unique_ptr<IScene>()>> m_factories;

    std::unique_ptr<IScene> m_current;
    SceneID                 m_pending = SceneID::None;
    bool                    m_hasPending = false;
};

} // namespace engine
