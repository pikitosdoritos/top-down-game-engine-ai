#pragma once
#include "engine/Core/Types.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Color.hpp>
#include <vector>

namespace engine {

// ─────────────────────────────────────────────────────────────────────────────
// FogOfWar
//
// Simple per-tile visibility grid rendered as a coloured VertexArray overlay.
// Visibility states:
//   Hidden    — player has never visited: fully dark
//   Explored  — player was here before: dim translucent
//   Visible   — currently in player's reveal radius: clear
//
// Rendered in world space after the tilemap, before entities.
// ─────────────────────────────────────────────────────────────────────────────

class FogOfWar {
public:
    enum class TileVis : uint8_t { Hidden = 0, Explored, Visible };

    void init(int cols, int rows, int tileW, int tileH);
    void reveal(Vec2f worldPos, float radius);   // call each frame with player pos
    void draw(sf::RenderWindow& window) const;

    // Colours
    sf::Color colHidden   { 0,   0,   0,   255 };
    sf::Color colExplored { 0,   0,   0,   160 };

private:
    void rebuildVertices() const;

    std::vector<TileVis> m_grid;
    int  m_cols  = 0;
    int  m_rows  = 0;
    int  m_tileW = 32;
    int  m_tileH = 32;

    // Keep dirty flag so we only rebuild on change
    mutable bool m_dirty = true;
    mutable sf::VertexArray m_vertices;
};

} // namespace engine
