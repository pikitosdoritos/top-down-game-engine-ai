#pragma once
#include "engine/Window/Window.hpp"
#include "engine/Input/InputManager.hpp"
#include "engine/Scene/SceneManager.hpp"
#include "engine/Resources/ResourceManager.hpp"
#include "engine/Audio/AudioManager.hpp"
#include "engine/Renderer/Renderer.hpp"

#include <string>
#include <memory>

namespace engine {

struct EngineConfig {
    std::string title       = "Game";
    unsigned    windowWidth  = 1280;
    unsigned    windowHeight = 720;
    unsigned    targetFPS    = 60;
    bool        vsync        = true;
    bool        fullscreen   = false;
};

class GameEngine {
public:
    explicit GameEngine(const EngineConfig& cfg = {});
    ~GameEngine() = default;

    // Non-copyable
    GameEngine(const GameEngine&)            = delete;
    GameEngine& operator=(const GameEngine&) = delete;

    void run();
    void quit();

    // Subsystem accessors (passed to scenes)
    Window&          window()    { return m_window; }
    InputManager&    input()     { return m_input; }
    SceneManager&    scenes()    { return m_scenes; }
    ResourceManager& resources() { return m_resources; }
    AudioManager&    audio()     { return m_audio; }
    Renderer&        renderer()  { return m_renderer; }

private:
    void processEvents();
    void update(float dt);
    void render();

    EngineConfig     m_config;
    Window           m_window;
    InputManager     m_input;
    ResourceManager  m_resources;
    AudioManager     m_audio;
    Renderer         m_renderer;
    SceneManager     m_scenes;
    bool             m_running = false;
};

} // namespace engine
