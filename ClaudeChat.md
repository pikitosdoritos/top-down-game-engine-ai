
Phase 1 — Architecture & Planning
Architecture Proposal
The engine is split into two independent CMake targets: engine (a static library) and game_demo (an executable that links against it). This lets future games swap in by replacing game_demo without touching engine code.

Module Breakdown

Engine (static lib)
├── Core          — GameEngine class, main loop, delta time
├── Window        — SFML window wrapper, vsync, resize
├── Input         — Keyboard/mouse polling, action mapping
├── Scene         — SceneManager + IScene interface (Menu, Playing, GameOver)
├── Entity        — Entity base class + component attachment
├── Components    — Transform, Sprite, Animation, Health, Collider, AI, Velocity
├── Renderer      — Wraps sf::RenderWindow draw calls, layer ordering
├── Animation     — Frame-based sprite sheet animator
├── Collision     — AABB broad-phase + resolution
├── Camera        — sf::View following with deadzone/lerp
├── Tilemap       — JSON-driven tile grid, culled rendering
├── Audio         — Sound effect / music wrapper around SFML audio
├── Resources     — Typed asset cache (textures, fonts, sounds)
└── UI            — HealthBar, Label, Screen overlays

Demo Game (executable)
├── Scenes        — MenuScene, GameScene, GameOverScene
├── Entities      — Player, Enemy, Projectile, WorldTile
└── assets/       — Sprites, tilemaps, audio, fonts (placeholder shapes as fallback)
Ownership Model

GameEngine owns → SceneManager
SceneManager owns → active IScene (unique_ptr swap on transition)
IScene owns → vector<Entity>
Entity owns → map of Components (by type_index)
Resources is a singleton cache; Engine holds one instance
Game Loop

while running:
  dt = clock.restart()
  engine.processInput()        // poll SFML events → Input
  engine.update(dt)            // SceneManager → active scene → entities
  engine.render()              // clear → SceneManager.draw → display
Delta time is capped at 100ms to prevent spiral-of-death on pause/resume.

Folder Structure

top-down-game-engine-ai/
├── CMakeLists.txt                  ← root: finds SFML, adds subdirs
├── README.md
│
├── engine/
│   ├── CMakeLists.txt              ← builds engine static lib
│   ├── include/engine/
│   │   ├── Core/
│   │   │   ├── GameEngine.hpp
│   │   │   └── Types.hpp
│   │   ├── Window/
│   │   │   └── Window.hpp
│   │   ├── Input/
│   │   │   └── InputManager.hpp
│   │   ├── Scene/
│   │   │   ├── IScene.hpp
│   │   │   └── SceneManager.hpp
│   │   ├── Entity/
│   │   │   ├── Entity.hpp
│   │   │   └── Component.hpp
│   │   ├── Components/
│   │   │   ├── TransformComponent.hpp
│   │   │   ├── SpriteComponent.hpp
│   │   │   ├── AnimationComponent.hpp
│   │   │   ├── HealthComponent.hpp
│   │   │   ├── ColliderComponent.hpp
│   │   │   ├── VelocityComponent.hpp
│   │   │   └── AIComponent.hpp
│   │   ├── Renderer/
│   │   │   └── Renderer.hpp
│   │   ├── Animation/
│   │   │   └── Animator.hpp
│   │   ├── Collision/
│   │   │   └── CollisionSystem.hpp
│   │   ├── Camera/
│   │   │   └── Camera.hpp
│   │   ├── Tilemap/
│   │   │   └── Tilemap.hpp
│   │   ├── Audio/
│   │   │   └── AudioManager.hpp
│   │   ├── Resources/
│   │   │   └── ResourceManager.hpp
│   │   └── UI/
│   │       ├── HealthBar.hpp
│   │       └── Label.hpp
│   └── src/
│       ├── Core/GameEngine.cpp
│       ├── Window/Window.cpp
│       ├── Input/InputManager.cpp
│       ├── Scene/SceneManager.cpp
│       ├── Entity/Entity.cpp
│       ├── Components/AnimationComponent.cpp
│       ├── Components/ColliderComponent.cpp
│       ├── Renderer/Renderer.cpp
│       ├── Collision/CollisionSystem.cpp
│       ├── Camera/Camera.cpp
│       ├── Tilemap/Tilemap.cpp
│       ├── Audio/AudioManager.cpp
│       ├── Resources/ResourceManager.cpp
│       └── UI/HealthBar.cpp
│
└── game_demo/
    ├── CMakeLists.txt              ← builds demo exe, links engine
    ├── assets/
    │   ├── fonts/
    │   ├── textures/               ← placeholder PNGs or SFML shapes
    │   ├── tilemaps/               ← JSON tile definitions
    │   └── audio/
    └── src/
        ├── main.cpp
        ├── Scenes/
        │   ├── MenuScene.hpp/cpp
        │   ├── GameScene.hpp/cpp
        │   └── GameOverScene.hpp/cpp
        └── Entities/
            ├── Player.hpp/cpp
            ├── Enemy.hpp/cpp
            └── Projectile.hpp/cpp
Dependency List
Dependency	Version	Purpose	How to get
SFML	2.6.x	Window, renderer, input, audio, image loading	vcpkg install sfml or download prebuilt
CMake	3.20+	Build system	cmake.org
nlohmann/json	3.x	Tilemap + config JSON parsing	header-only, bundled in repo
C++17	—	std::optional, structured bindings, filesystem	MSVC 2019+ / GCC 9+ / Clang 10+
No ECS framework (EnTT etc.) — a lightweight hand-rolled entity/component model keeps the engine learnable. No physics engine — AABB collision is sufficient for a Diablo-like.

Why SFML over SDL2?

SFML has a C++ native API (no void* casts), built-in sprite/texture/font/audio classes, and ships with everything needed in one package. SDL2 requires separate mixer/image/ttf libs and a more C-style API. For a learning-friendly engine, SFML wins.

Implementation Roadmap

Phase 2 — Scaffold
  [1] Root CMakeLists.txt
  [2] engine/CMakeLists.txt
  [3] game_demo/CMakeLists.txt
  [4] All engine headers (skeleton)
  [5] main.cpp + stub scenes

Phase 3 — Engine Implementation
  [6]  Types.hpp (Vec2, Rect, enums)
  [7]  Window
  [8]  InputManager
  [9]  ResourceManager
  [10] Entity + Component base
  [11] All Components
  [12] CollisionSystem
  [13] Camera
  [14] Tilemap (JSON)
  [15] AudioManager
  [16] Renderer
  [17] UI (HealthBar, Label)
  [18] SceneManager + IScene
  [19] GameEngine (ties it all together)

Phase 4 — Demo Game
  [20] Player entity (movement, attack, animation)
  [21] Enemy entity (AI, health, death)
  [22] Projectile entity
  [23] GameScene (tilemap, spawning, camera, HUD)
  [24] MenuScene + GameOverScene
  [25] Tilemap JSON asset

Phase 5 — Polish + README
  [26] Build verification pass
  [27] README.md