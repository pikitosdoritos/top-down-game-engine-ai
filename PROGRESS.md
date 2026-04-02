# Project Progress — Dungeon of Despair Engine

## What this project is

A reusable top-down 2D game engine in C++17 + SFML 3, with a playable demo game
inspired by early Diablo. The engine is a static library; the demo game links against it.
Future games can be added alongside `game_demo/` without touching the engine.

---

## Tech stack

| Tool | Version | Role |
|------|---------|------|
| C++ | 17 | Language |
| SFML | 3.0.2 (MinGW64) | Window, rendering, input, audio |
| CMake | 3.20+ | Build system |
| Ninja | — | Build backend |
| nlohmann/json | 3.11.3 (bundled) | Tilemap + config parsing |
| GCC (MSYS2) | MinGW64 | Compiler |

---

## Phase 1 — Planning ✅

- Defined engine architecture: 13 modules as a static library
- Defined folder structure: `engine/`, `game_demo/`, `third_party/`
- Chose SFML 3 over SDL2 (native C++ API, single package)
- Chose hand-rolled entity/component over full ECS (learnable, sufficient)
- Wrote implementation roadmap

---

## Phase 2 — Scaffold ✅

All files compile. `libengine.a` links successfully.

### Engine modules implemented (headers + sources)

| Module | Files | Status |
|--------|-------|--------|
| Core / GameEngine | `GameEngine.hpp/cpp`, `Types.hpp` | Done |
| Window | `Window.hpp/cpp` | Done |
| Input | `InputManager.hpp/cpp` | Done |
| Scene | `IScene.hpp`, `SceneManager.hpp/cpp` | Done |
| Entity | `Entity.hpp/cpp`, `Component.hpp` | Done |
| Components | Transform, Velocity, Health, Collider, Sprite, Animation, AI | Done |
| Renderer | `Renderer.hpp/cpp` | Done |
| Collision | `CollisionSystem.hpp/cpp` (AABB) | Done |
| Camera | `Camera.hpp/cpp` (lerp + bounds clamp) | Done |
| Tilemap | `Tilemap.hpp/cpp` (JSON + procedural) | Done |
| Audio | `AudioManager.hpp/cpp` | Done |
| Resources | `ResourceManager.hpp/cpp` (typed cache) | Done |
| UI | `HealthBar.hpp/cpp`, `Label.hpp/cpp` | Done |

### Game demo scaffold

| File | Status |
|------|--------|
| `main.cpp` | Done — registers 3 scenes, launches engine |
| `Scenes/MenuScene.hpp` | Header done |
| `Scenes/GameScene.hpp` | Header done |
| `Scenes/GameOverScene.hpp` | Header done |
| `Entities/Player.hpp` | Header done |
| `Entities/Enemy.hpp` | Header done |
| `Entities/Projectile.hpp` | Header done |
| `assets/fonts/Monocraft.ttf` | Downloaded |
| `assets/tilemaps/dungeon.json` | Hand-crafted (30×22 tile room) |

### Key SFML 3 adaptations made

SFML 3.0 has significant breaking changes from 2.x. All were handled:

- **Events** — variant-based; `event.getIf<sf::Event::KeyPressed>()` pattern
- **VideoMode** — takes `sf::Vector2u`: `sf::VideoMode({w, h})`
- **FloatRect** — members renamed: `.position` / `.size` (no more `.left/.top/.width/.height`)
  and constructor is `FloatRect({x,y}, {w,h})`
- **Sprite / Text** — no default constructor; stored as `std::optional<sf::Sprite>`
- **Font** — `openFromFile()` replaces `loadFromFile()`
- **Sound** — constructor requires buffer; status is `sf::SoundSource::Status::Stopped`
- **Music** — `setLooping()` replaces `setLoop()`
- **Fullscreen** — `sf::State::Fullscreen` (separate from `sf::Style`)
- **PrimitiveType** — `sf::PrimitiveType::Triangles` (Quads removed in SFML 3)
- **CMake targets** — `SFML::Graphics` etc. (not `sfml-graphics`)

---

## Phase 3 — Demo Game Implementation ✅

All `.cpp` files for the demo game still need to be written.

### To implement

#### Scenes
- `MenuScene.cpp` — title screen, blink prompt, press Enter → Game
- `GameScene.cpp` — tilemap render, spawn player + enemies, collision,
  camera follow, HUD (health bar + kill counter), detect win/lose conditions
- `GameOverScene.cpp` — "You Died" / "Victory" screen, press Enter → Menu

#### Entities
- `Player.cpp`
  - WASD movement with delta-time
  - Left-click projectile (or F key sword swing)
  - Attack hitbox + damage application
  - Idle / walk / attack / dead visual states (colored shapes)
  - `HealthComponent` integration
  - `ColliderComponent` + tile collision response

- `Enemy.cpp`
  - `AIComponent` — Idle → Chase → Attack state machine
  - Chase player when in detection range (400 px)
  - Attack when in melee range (50 px), 1 s cooldown, 10 dmg/hit
  - Float HP bar drawn above enemy
  - Hurt flash (red) on damage
  - Death: stop, fade color, mark `destroyed`

- `Projectile.cpp`
  - Flies in mouse-aim direction at 400 px/s
  - `ColliderComponent` (trigger) fires callback on enemy hit
  - Lifetime 1.5 s then `destroyed`

#### HUD
- Player HP bar (bottom-left, screen space)
- Kill counter label (top-right)
- "All enemies dead — Victory!" overlay

---

## Phase 4 — Polish + README ✅

- Verify complete build: `cmake --build build`
- Run and smoke-test the game
- Write `README.md`:
  - Project overview + architecture diagram
  - Build steps (Windows / Linux)
  - Controls
  - How to extend the engine (add a new game, add a component)

---

## Build instructions (current state)

```bash
# Prerequisites: MSYS2 with mingw-w64-x86_64-sfml installed
# (already done on this machine)

cd top-down-game-engine-ai

cmake -B build -G Ninja \
  -DCMAKE_CXX_COMPILER=C:/msys64/mingw64/bin/g++.exe \
  -DCMAKE_C_COMPILER=C:/msys64/mingw64/bin/gcc.exe \
  -DSFML_DIR=C:/msys64/mingw64/lib/cmake/SFML

cmake --build build

# Run:
./build/bin/game_demo.exe
```

---

## Controls (planned)

| Key / Button | Action |
|---|---|
| W A S D | Move |
| Left Mouse | Fire projectile toward cursor |
| F | Melee sword swing |
| Enter | Confirm on menu/game-over screens |
| Escape | Quit |
