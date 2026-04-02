add_library(engine STATIC
    src/Core/GameEngine.cpp
    src/Window/Window.cpp
    src/Input/InputManager.cpp
    src/Scene/SceneManager.cpp
    src/Entity/Entity.cpp
    src/Components/AnimationComponent.cpp
    src/Components/ColliderComponent.cpp
    src/Components/HealthComponent.cpp
    src/Components/SpriteComponent.cpp
    src/Components/AIComponent.cpp
    src/Renderer/Renderer.cpp
    src/Collision/CollisionSystem.cpp
    src/Camera/Camera.cpp
    src/Tilemap/Tilemap.cpp
    src/Audio/AudioManager.cpp
    src/Resources/ResourceManager.cpp
    src/UI/HealthBar.cpp
    src/UI/Label.cpp
)

target_include_directories(engine
    PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(engine
    PUBLIC
        SFML::System
        SFML::Window
        SFML::Graphics
        SFML::Audio
        nlohmann_json
)

if(MSVC)
    target_compile_options(engine PRIVATE /W4)
else()
    target_compile_options(engine PRIVATE -Wall -Wextra -Wpedantic)
endif()
