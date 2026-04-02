cmake_minimum_required(VERSION 3.20)
project(TopDownEngine VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Output binaries to build/bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# ── SFML 3 ───────────────────────────────────────────────────────────────────
# Targets: SFML::System, SFML::Window, SFML::Graphics, SFML::Audio
# Set SFML_DIR if not found automatically:
#   cmake .. -DSFML_DIR=C:/msys64/mingw64/lib/cmake/SFML
find_package(SFML 3 REQUIRED COMPONENTS System Window Graphics Audio)

# ── nlohmann/json (header-only, bundled) ─────────────────────────────────────
add_library(nlohmann_json INTERFACE)
target_include_directories(nlohmann_json INTERFACE
    ${CMAKE_SOURCE_DIR}/third_party
)

add_subdirectory(engine)
add_subdirectory(game_demo)
