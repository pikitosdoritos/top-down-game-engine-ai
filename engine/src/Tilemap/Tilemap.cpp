#include "engine/Tilemap/Tilemap.hpp"
#include <nlohmann/json.hpp>
#include <fstream>
#include <cmath>
#include <algorithm>

namespace engine {

bool Tilemap::loadFromFile(const std::string& jsonPath, const TilesetInfo& tileset)
{
    std::ifstream f(jsonPath);
    if (!f.is_open()) return false;

    nlohmann::json j;
    try { f >> j; } catch (...) { return false; }

    m_tileW = j.value("tileWidth",  tileset.tileWidth);
    m_tileH = j.value("tileHeight", tileset.tileHeight);
    m_cols  = j.value("columns",    0);
    m_rows  = j.value("rows",       0);

    m_tiles     = j.at("tiles").get<std::vector<int>>();
    m_collision = j.value("collision", std::vector<int>(m_tiles.size(), 0));

    m_texture = tileset.texture;
    buildVertices(tileset);
    return true;
}

void Tilemap::buildRoom(int cols, int rows, int tileW, int tileH,
                        const TilesetInfo& tileset,
                        int floorTile, int wallTile)
{
    m_cols  = cols;
    m_rows  = rows;
    m_tileW = tileW;
    m_tileH = tileH;
    m_texture = tileset.texture;

    m_tiles.resize(cols * rows);
    m_collision.resize(cols * rows, 0);

    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            bool isWall = (x == 0 || y == 0 || x == cols - 1 || y == rows - 1);
            int idx = y * cols + x;
            m_tiles[idx]     = isWall ? wallTile : floorTile;
            m_collision[idx] = isWall ? 1 : 0;
        }
    }

    buildVertices(tileset);
}

void Tilemap::buildDungeon(int cols, int rows, int tileW, int tileH,
                           const TilesetInfo& tileset,
                           int floorTile, int wallTile)
{
    m_cols  = cols;
    m_rows  = rows;
    m_tileW = tileW;
    m_tileH = tileH;
    m_texture = tileset.texture;

    m_tiles.assign(cols * rows, wallTile);
    m_collision.assign(cols * rows, 1);

    struct Room { int x, y, w, h; };
    std::vector<Room> rooms;

    int numRooms = 5 + (cols * rows) / 400; // room density
    for (int i = 0; i < numRooms; ++i) {
        int rw = 4 + rand() % 6;
        int rh = 4 + rand() % 6;
        int rx = 1 + rand() % (cols - rw - 1);
        int ry = 1 + rand() % (rows - rh - 1);

        // Simple overlap check
        bool overlap = false;
        for (const auto& r : rooms) {
            if (rx < r.x + r.w + 1 && rx + rw + 1 > r.x &&
                ry < r.y + r.h + 1 && ry + rh + 1 > r.y) {
                overlap = true;
                break;
            }
        }
        if (overlap) continue;

        // Carve room
        for (int y = ry; y < ry + rh; ++y) {
            for (int x = rx; x < rx + rw; ++x) {
                int idx = y * cols + x;
                m_tiles[idx]     = floorTile;
                m_collision[idx] = 0;
            }
        }

        // Corridor to previous room
        if (!rooms.empty()) {
            Room& prev = rooms.back();
            int cx1 = rx + rw / 2;
            int cy1 = ry + rh / 2;
            int cx2 = prev.x + prev.w / 2;
            int cy2 = prev.y + prev.h / 2;

            // L-shape corridor
            auto carveH = [&](int x1, int x2, int y) {
                for (int x = std::min(x1, x2); x <= std::max(x1, x2); ++x) {
                    m_tiles[y * cols + x]     = floorTile;
                    m_collision[y * cols + x] = 0;
                }
            };
            auto carveV = [&](int y1, int y2, int x) {
                for (int y = std::min(y1, y2); y <= std::max(y1, y2); ++y) {
                    m_tiles[y * m_cols + x]     = floorTile;
                    m_collision[y * m_cols + x] = 0;
                }
            };

            if (rand() % 2 == 0) {
                carveH(cx1, cx2, cy1);
                carveV(cy1, cy2, cx2);
            } else {
                carveV(cy1, cy2, cx1);
                carveH(cx1, cx2, cy2);
            }
        }
        rooms.push_back({rx, ry, rw, rh});
    }

    buildVertices(tileset);
}

void Tilemap::buildVertices(const TilesetInfo& tileset)
{
    m_solidRects.clear();
    m_vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
    m_vertices.resize(m_cols * m_rows * 6); // 2 triangles per tile

    int sheetCols = tileset.columns > 0 ? tileset.columns : 1;

    for (int y = 0; y < m_rows; ++y) {
        for (int x = 0; x < m_cols; ++x) {
            int idx  = y * m_cols + x;
            int tile = m_tiles[idx];

            float wx = static_cast<float>(x * m_tileW);
            float wy = static_cast<float>(y * m_tileH);
            float ww = static_cast<float>(m_tileW);
            float wh = static_cast<float>(m_tileH);

            int tileCol = tile % sheetCols;
            int tileRow = tile / sheetCols;
            float tw = static_cast<float>(tileset.tileWidth);
            float th = static_cast<float>(tileset.tileHeight);
            
            // Add slight UV padding to avoid bleeding/seams
            float u  = static_cast<float>(tileCol * tileset.tileWidth) + 0.1f;
            float v  = static_cast<float>(tileRow * tileset.tileHeight) + 0.1f;
            tw -= 0.2f;
            th -= 0.2f;

            // Default fallback colors
            sf::Color tileColor = sf::Color::White;
            if (!m_texture) {
                tileColor = (m_collision[idx])
                    ? sf::Color(60, 40, 80)   // wall
                    : sf::Color(35, 25, 50);  // floor
            }

            // 6 vertices (2 triangles)
            sf::Vertex* tri = &m_vertices[idx * 6];

            // Triangle 1
            tri[0] = {{wx,      wy},      tileColor, {u,      v}};
            tri[1] = {{wx + ww, wy},      tileColor, {u + tw, v}};
            tri[2] = {{wx,      wy + wh}, tileColor, {u,      v + th}};
            // Triangle 2
            tri[3] = {{wx + ww, wy},      tileColor, {u + tw, v}};
            tri[4] = {{wx + ww, wy + wh}, tileColor, {u + tw, v + th}};
            tri[5] = {{wx,      wy + wh}, tileColor, {u,      v + th}};

            if (m_collision[idx]) {
                m_solidRects.push_back({{wx, wy}, {ww, wh}});
            }
        }
    }
}

void Tilemap::draw(sf::RenderWindow& window) const
{
    if (m_texture) {
        sf::RenderStates states;
        states.texture = m_texture;
        window.draw(m_vertices, states);
    } else {
        window.draw(m_vertices);
    }
}

FloatRect Tilemap::worldBounds() const
{
    return {{0.f, 0.f},
            {static_cast<float>(m_cols * m_tileW),
             static_cast<float>(m_rows * m_tileH)}};
}

bool Tilemap::hasLineOfSight(Vec2f from, Vec2f to) const
{
    // DDA (Digital Differential Analyzer) grid traversal
    float tileW = static_cast<float>(m_tileW);
    float tileH = static_cast<float>(m_tileH);

    int tx0 = static_cast<int>(from.x / tileW);
    int ty0 = static_cast<int>(from.y / tileH);
    int tx1 = static_cast<int>(to.x   / tileW);
    int ty1 = static_cast<int>(to.y   / tileH);

    int dx = tx1 - tx0;
    int dy = ty1 - ty0;
    int stepX = (dx > 0) ? 1 : (dx < 0 ? -1 : 0);
    int stepY = (dy > 0) ? 1 : (dy < 0 ? -1 : 0);

    float rayDirX = to.x - from.x;
    float rayDirY = to.y - from.y;

    float tDeltaX = (rayDirX != 0.f) ? std::abs(tileW / rayDirX) : 1e30f;
    float tDeltaY = (rayDirY != 0.f) ? std::abs(tileH / rayDirY) : 1e30f;

    float tMaxX, tMaxY;
    if (stepX > 0)
        tMaxX = ((tx0 + 1) * tileW - from.x) / rayDirX;
    else if (stepX < 0)
        tMaxX = (tx0 * tileW - from.x) / rayDirX;
    else
        tMaxX = 1e30f;

    if (stepY > 0)
        tMaxY = ((ty0 + 1) * tileH - from.y) / rayDirY;
    else if (stepY < 0)
        tMaxY = (ty0 * tileH - from.y) / rayDirY;
    else
        tMaxY = 1e30f;

    int cx = tx0, cy = ty0;
    int maxSteps = std::abs(dx) + std::abs(dy) + 2;

    for (int i = 0; i < maxSteps; ++i) {
        if (cx == tx1 && cy == ty1) break;
        if (isSolid(cx, cy)) return false;

        if (tMaxX < tMaxY) {
            tMaxX += tDeltaX;
            cx    += stepX;
        } else {
            tMaxY += tDeltaY;
            cy    += stepY;
        }
    }
    return !isSolid(tx1, ty1);
}

} // namespace engine
