# Dungeon of Despair (Top-Down Engine)

A lightweight, reusable 2D top-down game engine written in modern C++17, built on top of SFML 3. It includes a playable demo game inspired by early Diablo prototypes.

## Overview

The purpose of this project is to provide a clean, extensible, and understandable game engine architecture without relying on heavy frameworks. It separates the **engine code** from the **game-specific code**, meaning you can reuse the engine for different top-down (or 2D) games simply by swapping out the executable target.

### Features
- **Engine Module**: A static library (`libengine`) containing all core subsystems.
- **Game Demo Module**: A separate executable (`game_demo`) that links against the engine.
- **Component-based Entities**: A simple bespoke ECS (Entity-Component-System) structure optimized for readability over raw performance.
- **Subsystems**:
  - `GameEngine` & `Window` (Core Loop, VSync, framerate capping)
  - `SceneManager` (Menu, Gameplay, Game Over states)
  - `Renderer` & `Camera` (Resolution management, layers, deadzone tracking)
  - `Collision` (AABB physics and procedural tile mapping)
  - `InputManager` & `AudioManager`
  - `ResourceManager` (Asset caching for textures, fonts, sounds)
  - `UI` (Basic HUDs, health bars, floating labels)

## Architecture Summary

The engine avoids global god classes in favor of a clear ownership tree:
- **`GameEngine`** owns the window, input, and `SceneManager`.
- **`SceneManager`** swaps and manages the lifecycles of `IScene` objects.
- **`IScene`** instances own a standard contiguous `vector<Entity>`.
- **`Entity`** objects own a map of lightweight `Component` objects (e.g. `TransformComponent`, `SpriteComponent`, `HealthComponent`).

## Controls (Demo)

| Key / Mouse   | Action                          |
|---------------|---------------------------------|
| **W, A, S, D**| Move Player                     |
| **Left Click**| Fire Projectile toward cursor   |
| **F**         | Melee Attack (Sword Swing)      |
| **Enter**     | Start Game / Restart (Menus)     |
| **Escape**    | Quit Game                       |

## Build Instructions

This project uses CMake to organize the build process and download dependencies like SFML (if managed through vcpkg) or relies on system installations.

### Windows (MSYS2 + MinGW64)
Ensure you have installed MSYS2 along with `mingw-w64-x86_64-gcc`, `mingw-w64-x86_64-cmake`, `mingw-w64-x86_64-ninja`, and `mingw-w64-x86_64-sfml`.

```bash
cd top-down-game-engine-ai

# Generate Ninja build files
cmake -B build -G Ninja \
  -DCMAKE_CXX_COMPILER=C:/msys64/mingw64/bin/g++.exe \
  -DCMAKE_C_COMPILER=C:/msys64/mingw64/bin/gcc.exe \
  -DSFML_DIR=C:/msys64/mingw64/lib/cmake/SFML

# Compile the project
cmake --build build

# Run the demo (make sure to run from within the build directory to resolve assets properly)
cd build/bin
./game_demo.exe
```

### Linux
Ensure you have the base development packages, CMake, and SFML 3 development headers installed (e.g. `libsfml-dev` or compiled from source if your package manager only has SFML 2.x).

```bash
cd top-down-game-engine-ai

# Generate standard Unix Makefiles or Ninja
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

# Compile
cmake --build build -j$(nproc)

# Run the demo
cd build/bin
./game_demo
```

## How to Extend the Engine

### Adding a New Target Game
1. Duplicate the `game_demo/` folder and rename it (e.g., `space_shooter/`).
2. Add a new `add_subdirectory(space_shooter)` to the root `CMakeLists.txt`.
3. Inside `space_shooter/CMakeLists.txt`, change the target name and define your new game's `.cpp` source files.
4. Replace the source code to implement your own scenes and entities!

### Adding a Custom Component
1. Create a header `MyComponent.hpp` inside `engine/include/engine/Components/`. It should inherit from `Component`.
2. Add fields. For instance, `float poisonDamage = 5.0f;`.
3. In your game code, simply call:  
   `auto* poison = entity->addComponent<MyComponent>();`
4. Create a system (like `CollisionSystem.cpp`) or iterate over entities in `GameScene::update` to find entities `hasComponent<MyComponent>()` and apply the game logic.
