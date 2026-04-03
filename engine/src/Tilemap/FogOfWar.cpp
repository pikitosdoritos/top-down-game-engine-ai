#include "engine/Tilemap/FogOfWar.hpp"
#include <cmath>

namespace engine {

void FogOfWar::init(int cols, int rows, int tileW, int tileH)
{
    m_cols  = cols;
    m_rows  = rows;
    m_tileW = tileW;
    m_tileH = tileH;
    m_grid.assign(cols * rows, TileVis::Hidden);
    m_dirty = true;
}

void FogOfWar::reveal(Vec2f worldPos, float radius)
{
    // Tile under player
    int cx = static_cast<int>(worldPos.x / m_tileW);
    int cy = static_cast<int>(worldPos.y / m_tileH);

    int tileRadius = static_cast<int>(std::ceil(radius / m_tileW)) + 1;

    for (int dy = -tileRadius; dy <= tileRadius; ++dy) {
        for (int dx = -tileRadius; dx <= tileRadius; ++dx) {
            int tx = cx + dx;
            int ty = cy + dy;
            if (tx < 0 || ty < 0 || tx >= m_cols || ty >= m_rows) continue;

            // World-space centre of this tile
            float wx = (tx + 0.5f) * m_tileW;
            float wy = (ty + 0.5f) * m_tileH;
            float dist = std::hypot(wx - worldPos.x, wy - worldPos.y);

            int idx = ty * m_cols + tx;
            TileVis& vis = m_grid[idx];

            if (dist <= radius) {
                if (vis != TileVis::Visible) {
                    vis = TileVis::Visible;
                    m_dirty = true;
                }
            } else if (vis == TileVis::Hidden) {
                // Mark outer ring as explored so border is not black at edge
                // (soft reveal optional — keep simple)
            }
        }
    }

    // Downgrade previously-visible tiles outside radius to Explored
    for (int ty = 0; ty < m_rows; ++ty) {
        for (int tx = 0; tx < m_cols; ++tx) {
            float wx = (tx + 0.5f) * m_tileW;
            float wy = (ty + 0.5f) * m_tileH;
            float dist = std::hypot(wx - worldPos.x, wy - worldPos.y);
            int idx = ty * m_cols + tx;
            if (dist > radius && m_grid[idx] == TileVis::Visible) {
                m_grid[idx] = TileVis::Explored;
                m_dirty = true;
            }
        }
    }
}

void FogOfWar::rebuildVertices() const
{
    m_vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
    m_vertices.resize(m_cols * m_rows * 6);

    for (int y = 0; y < m_rows; ++y) {
        for (int x = 0; x < m_cols; ++x) {
            TileVis v = m_grid[y * m_cols + x];
            if (v == TileVis::Visible) continue; // fully transparent — skip alpha

            sf::Color col = (v == TileVis::Hidden) ? colHidden : colExplored;

            float wx = static_cast<float>(x * m_tileW);
            float wy = static_cast<float>(y * m_tileH);
            float ww = static_cast<float>(m_tileW);
            float wh = static_cast<float>(m_tileH);

            sf::Vertex* tri = &m_vertices[(y * m_cols + x) * 6];
            tri[0] = {{wx,      wy},      col};
            tri[1] = {{wx + ww, wy},      col};
            tri[2] = {{wx,      wy + wh}, col};
            tri[3] = {{wx + ww, wy},      col};
            tri[4] = {{wx + ww, wy + wh}, col};
            tri[5] = {{wx,      wy + wh}, col};
        }
    }
    m_dirty = false;
}

void FogOfWar::draw(sf::RenderWindow& window) const
{
    if (m_dirty) rebuildVertices();
    window.draw(m_vertices);
}

bool FogOfWar::isVisible(Vec2f worldPos) const
{
    if (m_cols <= 0 || m_rows <= 0) return true; // fallback

    int tx = static_cast<int>(worldPos.x / m_tileW);
    int ty = static_cast<int>(worldPos.y / m_tileH);

    if (tx < 0 || tx >= m_cols || ty < 0 || ty >= m_rows) 
        return false;

    return m_grid[ty * m_cols + tx] == TileVis::Visible;
}

} // namespace engine
