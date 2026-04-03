#pragma once
#include "engine/Core/Types.hpp"
#include <string>

// ─────────────────────────────────────────────────────────────────────────────
// CampaignState  —  plain shared struct with no dependencies.
// Stored as a global singleton accessed via CampaignState::get().
// Each level scene reads from this to configure itself.
// ─────────────────────────────────────────────────────────────────────────────

struct LevelConfig {
    // Map geometry
    int   mapCols       = 30;
    int   mapRows       = 22;
    int   tileSize      = 32;

    // Tileset texture name (loaded via ResourceManager)
    std::string tilesetAsset;   // e.g. "tileset_level1"

    // Floor / wall colours (used when no texture loaded)
    unsigned floorR = 35,  floorG = 25,  floorB = 50;
    unsigned wallR  = 60,  wallG  = 40,  wallB  = 80;

    // Enemy setup (regular grunts)
    int   gruntCount  = 6;
    float gruntHP     = 60.f;
    float gruntSpeed  = 80.f;
    float gruntDamage = 10.f;

    // Brute enemies (level 2+)
    int   bruteCount  = 0;
    float bruteHP     = 140.f;
    float bruteSpeed  = 50.f;
    float bruteDamage = 22.f;

    // Objective
    std::string levelName;
    std::string objectiveText;

    // Which scene to go to after completing this level
    engine::SceneID nextScene = engine::SceneID::GameOver;

    // Ambient light colour (used for renderer clear colour)
    unsigned ambR = 10, ambG = 5, ambB = 15;
};

struct CampaignState {
    int currentLevel = 1;           // 1-based

    static const LevelConfig& config(int level);

    // Convenience
    static const LevelConfig& current() {
        return config(get().currentLevel);
    }

    static CampaignState& get() {
        static CampaignState s;
        return s;
    }

private:
    CampaignState() = default;
};
