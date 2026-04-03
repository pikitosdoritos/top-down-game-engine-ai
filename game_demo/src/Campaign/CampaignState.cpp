#include "Campaign/CampaignState.hpp"

// ─────────────────────────────────────────────────────────────────────────────
// Static level definitions.
// All visual differences, enemy counts, and objectives live here.
// ─────────────────────────────────────────────────────────────────────────────

const LevelConfig& CampaignState::config(int level)
{
    // ── LEVEL 1: Cursed Village ──────────────────────────────────────────────
    static const LevelConfig L1 = []{
        LevelConfig c;
        c.mapCols       = 28;
        c.mapRows       = 22;
        c.tileSize      = 32;
        c.tilesetAsset  = "tileset_l1";

        // Warm earthy palette — crumbling stone village
        c.floorR = 45;  c.floorG = 38;  c.floorB = 28;
        c.wallR  = 80;  c.wallG  = 60;  c.wallB  = 40;

        c.gruntCount  = 5;
        c.gruntHP     = 45.f;
        c.gruntSpeed  = 65.f;
        c.gruntDamage = 8.f;
        c.bruteCount  = 0;

        c.levelName    = "Level 1: Cursed Village";
        c.objectiveText = "Objective: Slay all enemies";
        c.nextScene    = engine::SceneID::Level2;

        // Warm amber ambient
        c.ambR = 18; c.ambG = 10; c.ambB = 5;
        return c;
    }();

    // ── LEVEL 2: Bone Crypts ─────────────────────────────────────────────────
    static const LevelConfig L2 = []{
        LevelConfig c;
        c.mapCols       = 38;
        c.mapRows       = 30;
        c.tileSize      = 32;
        c.tilesetAsset  = "tileset_l2";

        // Cold grey-blue — ancient crypt stone
        c.floorR = 28;  c.floorG = 30;  c.floorB = 42;
        c.wallR  = 55;  c.wallG  = 55;  c.wallB  = 80;

        c.gruntCount  = 7;
        c.gruntHP     = 60.f;
        c.gruntSpeed  = 90.f;
        c.gruntDamage = 12.f;

        c.bruteCount  = 3;   // NEW enemy type introduced here
        c.bruteHP     = 140.f;
        c.bruteSpeed  = 48.f;
        c.bruteDamage = 22.f;

        c.levelName    = "Level 2: Bone Crypts";
        c.objectiveText = "Objective: Slay all enemies";
        c.nextScene    = engine::SceneID::Level3;

        // Cold deep-blue ambient
        c.ambR = 5; c.ambG = 8; c.ambB = 22;
        return c;
    }();

    // ── LEVEL 3: The Abyss ───────────────────────────────────────────────────
    static const LevelConfig L3 = []{
        LevelConfig c;
        c.mapCols       = 48;
        c.mapRows       = 38;
        c.tileSize      = 32;
        c.tilesetAsset  = "tileset_l3";

        // Deep blood-red — demonic abyss
        c.floorR = 40;  c.floorG = 12;  c.floorB = 12;
        c.wallR  = 80;  c.wallG  = 20;  c.wallB  = 20;

        c.gruntCount  = 10;
        c.gruntHP     = 75.f;
        c.gruntSpeed  = 110.f;
        c.gruntDamage = 14.f;

        c.bruteCount  = 6;
        c.bruteHP     = 180.f;
        c.bruteSpeed  = 60.f;
        c.bruteDamage = 28.f;

        c.levelName    = "Level 3: The Abyss";
        c.objectiveText = "Objective: Slay all enemies and survive";
        c.nextScene    = engine::SceneID::CampaignVictory;

        // Dark crimson ambient
        c.ambR = 18; c.ambG = 3; c.ambB = 3;
        return c;
    }();

    switch (level) {
        case 1:  return L1;
        case 2:  return L2;
        default: return L3;
    }
}
