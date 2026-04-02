You are a senior C++ game engine developer and technical architect.

Your task is to build a small but clean, extensible, installable top-down 2D game engine in C++ with a playable demo game inspired by the feel of Diablo (top-down movement, combat, enemies, map, camera, basic RPG-style atmosphere), while keeping the project realistic and small enough to run locally and be understood by one developer.

Important constraints:
- The engine must be written in modern C++.
- Prefer a lightweight, practical stack.
- Use CMake as the build system.
- Make the engine easy to build and reuse for future games.
- The result must compile and run locally.
- The demo game must prove that the engine works.
- Keep architecture clean and modular, not overengineered.
- Explain decisions briefly as you go.

Recommended tech stack:
- C++17 or C++20
- SFML for windowing, rendering, input, audio
- CMake
- Organize the project so the engine and demo game are separated

Main goal:
Create a reusable top-down game engine plus a simple demo ARPG-style game that feels like an early Diablo prototype.

Project requirements

1. Engine architecture
Build the engine with clear modules such as:
- Core / Engine loop
- Window management
- Input system
- Scene or state management
- Entity system (simple is fine; full ECS is optional)
- Component-based game object structure if useful
- Renderer
- Animation system
- Collision system
- Camera system
- Tilemap support
- Audio manager
- Resource/asset manager
- UI/HUD basics
- Basic scripting/config-driven gameplay where practical via JSON or simple config files

2. Demo game features
Create a playable demo showing the engine in action:
- Top-down character movement using keyboard
- Mouse aiming if useful
- Basic attack system
- Enemies that move toward the player
- Enemy health
- Player health
- Collision with world and enemies
- Tile-based level
- Camera following the player
- Simple UI:
  - health bar
  - maybe enemy HP
  - start screen / game over screen
- At least one of:
  - sword melee attack
  - projectile attack
- Basic animation states:
  - idle
  - walk
  - attack
  - death for enemy if possible
- A dark fantasy / dungeon prototype feel similar to Diablo, even with placeholder shapes or primitive art

3. Installable / reusable structure
I want this to be usable for future games, so structure the project like this or similarly:

/engine
  /include
  /src
/game_demo
  /assets
  /src
/CMakeLists.txt
/README.md

Requirements:
- Engine code should be reusable from the demo project
- The demo should depend on the engine, not be mixed into it
- Provide clean instructions to build and run
- Make it possible to add future games using the same engine

4. Output format
Work in phases and do not jump chaotically.

Phase 1 — planning
- First, analyze the task
- Propose the architecture
- Propose the folder structure
- List dependencies
- Explain why this stack is a good fit

Phase 2 — scaffold
- Generate the full folder structure
- Generate all CMake files
- Generate starter engine classes and headers
- Generate the demo game entry point

Phase 3 — implementation
- Implement the engine step by step
- Keep files complete and production-like
- Do not leave fake TODO placeholders unless absolutely necessary
- If a file is updated, show the full final file

Phase 4 — demo gameplay
- Implement the playable demo
- Add movement, enemies, collisions, attack, camera, HUD
- Use placeholder assets if real assets are unavailable
- If assets are unavailable, use simple colored rectangles/circles/sprites and still make the game playable

Phase 5 — polish
- Improve code organization
- Remove obvious duplication
- Ensure build instructions are correct
- Write README with:
  - project overview
  - architecture summary
  - build steps
  - controls
  - how to extend the engine

5. Code quality requirements
- Use modern C++ style
- Use clear naming
- Separate headers/source files properly
- Avoid giant god classes when possible
- Keep the engine understandable for learning
- Add comments only where they help
- Keep the implementation practical, not academic
- Make sure the engine loop, update flow, rendering flow, and ownership model are clear

6. Gameplay target
The demo should feel like a mini Diablo-like prototype:
- top-down perspective
- dungeon-like arena or room
- player can move fluidly
- enemies chase and damage player
- player can attack and kill enemies
- basic combat feedback
- simple but satisfying gameplay loop

7. Technical expectations
Please include:
- delta time handling
- basic collision resolution
- sprite or shape rendering
- animation timing
- game states (menu, playing, game over)
- asset loading abstraction
- clean input polling
- camera view handling
- basic enemy AI chase behavior
- simple spawn setup
- health/damage system

8. Build and run expectations
- Use CMake properly
- Clearly specify SFML dependency
- Provide exact build steps for Windows
- Also provide build steps for Linux if practical
- Assume local development in VS Code or CLion is possible

9. Important working rules
- Do not give me only high-level advice
- Actually generate the codebase
- Be systematic
- Before writing code, present the plan
- Then implement file by file
- Make sure files are mutually consistent
- If you discover a better architecture while implementing, explain the change briefly and continue
- Prefer a minimal working engine over an unfinished ambitious one

10. Final deliverable
At the end I want:
- a reusable top-down 2D C++ engine
- a demo game built on top of it
- a structure suitable for future expansion
- a playable result with Diablo-like feel
- a README with setup and controls

Start with Phase 1:
1. Architecture proposal
2. Folder structure
3. Dependency list
4. Short implementation roadmap

Then continue into implementation after the plan.