add_executable(game_demo
    src/main.cpp
    src/Scenes/MenuScene.cpp
    src/Scenes/GameScene.cpp
    src/Scenes/GameOverScene.cpp
    src/Scenes/CampaignLevelScene.cpp
    src/Scenes/CampaignVictoryScene.cpp
    src/Entities/Player.cpp
    src/Entities/Enemy.cpp
    src/Entities/Brute.cpp
    src/Entities/Projectile.cpp
    src/Campaign/CampaignState.cpp
)

target_include_directories(game_demo
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(game_demo
    PRIVATE engine
)

# Copy assets next to the executable after build
add_custom_command(TARGET game_demo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:game_demo>/assets
    COMMENT "Copying game assets to build directory"
)

# On Windows with dynamically-linked SFML, copy DLLs next to the executable.
# With static SFML (recommended for distribution) this is a no-op.
if(WIN32)
    add_custom_command(TARGET game_demo POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:game_demo>
            $<TARGET_FILE_DIR:game_demo>
        COMMAND_EXPAND_LISTS
    )
endif()

if(MSVC)
    target_compile_options(game_demo PRIVATE /W4)
else()
    target_compile_options(game_demo PRIVATE -Wall -Wextra -Wpedantic)
endif()
