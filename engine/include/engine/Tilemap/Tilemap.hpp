#pragma once
#include "engine/Core/Types.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <vector>
#include <string>

namespace engine {

struct TilesetInfo {
    const sf::Texture* texture = nullptr;
    int tileWidth  = 32;
    int tileHeight = 32;
    int columns    = 1;   // tiles per row in the sheet
};

// Tilemap loaded from a simple JSON file.
// JSON schema:
// {
//   "tileWidth": 32, "tileHeight": 32,
//   "columns": 20, "rows": 15,
//   "tilesetColumns": 8,
//   "tiles": [0,1,2,...],   <- flat array, row-major
//   "collision": [0,1,...]  <- 1 = solid wall tile
// }
class Tilemap {
public:
    bool loadFromFile(const std::string& jsonPath, const TilesetInfo& tileset);

    // Build a procedural dungeon room (no JSON required)
    void buildRoom(int cols, int rows, int tileW, int tileH,
                   const TilesetInfo& tileset,
                   int floorTile = 0, int wallTile = 1);

    // Build a procedural dungeon with rooms and corridors (ADOM-style)
    void buildDungeon(int cols, int rows, int tileW, int tileH,
                      const TilesetInfo& tileset,
                      int floorTile = 0, int wallTile = 1);

    void draw(sf::RenderWindow& window) const;

    // Returns all solid AABB rects for collision
    const std::vector<FloatRect>& solidRects() const { return m_solidRects; }

    FloatRect worldBounds() const;

    int columns()    const { return m_cols; }
    int rows()       const { return m_rows; }
    int tileWidth()  const { return m_tileW; }
    int tileHeight() const { return m_tileH; }

    bool isSolid(int tx, int ty) const {
        if (tx < 0 || tx >= m_cols || ty < 0 || ty >= m_rows) return true;
        return m_collision[ty * m_cols + tx] != 0;
    }

    // DDA raycast: returns true if the straight line from 'from' to 'to' (world px)
    // does not pass through any solid tile.
    bool hasLineOfSight(Vec2f from, Vec2f to) const;

private:
    void buildVertices(const TilesetInfo& tileset);

    std::vector<int>      m_tiles;
    std::vector<int>      m_collision;
    int                   m_cols  = 0;
    int                   m_rows  = 0;
    int                   m_tileW = 32;
    int                   m_tileH = 32;
    sf::VertexArray       m_vertices;
    const sf::Texture*    m_texture = nullptr;
    std::vector<FloatRect> m_solidRects;
};

} // namespace engine
