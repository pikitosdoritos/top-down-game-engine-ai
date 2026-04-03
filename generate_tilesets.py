"""
Fast tileset generator using NumPy bulk operations.
Creates 3 themed 32x128 tilesets (4 tiles stacked: floor, wall, corridor, pillar).
Runtime: < 1 second.
"""
import numpy as np
from PIL import Image
import os

os.makedirs('game_demo/assets/textures', exist_ok=True)

def make_tileset(path, floor_rgb, wall_rgb, corridor_rgb):
    rng = np.random.default_rng(hash(path) & 0xFFFFFFFF)

    W, H = 32, 128
    img = np.zeros((H, W, 4), dtype=np.uint8)
    img[:, :, 3] = 255  # full alpha

    def noise(base, lo=-10, hi=10):
        n = rng.integers(lo, hi, size=(32, 32), dtype=np.int16)
        return np.clip(base + n, 0, 255).astype(np.uint8)

    # Tile 0: Room floor (y 0-31)
    for i, c in enumerate(floor_rgb):
        img[0:32, :, i] = noise(c, -8, 8)
    # subtle grid lines
    for k in range(0, 32, 8):
        for i, c in enumerate(floor_rgb):
            v = max(0, c - 20)
            img[k,    :, i] = v
            img[:, k,    i][0:32] = v  # only top tile rows

    # Tile 1: Wall with brick pattern (y 32-63)
    base_wall = np.zeros((32, 32, 3), dtype=np.uint8)
    for y in range(32):
        row_idx = y // 8
        offset = 0 if row_idx % 2 == 0 else 16
        for x in range(32):
            mortar_h = (y % 8 == 0)
            mortar_v = ((x + offset) % 16 == 0)
            if mortar_h or mortar_v:
                for i, c in enumerate(wall_rgb):
                    base_wall[y, x, i] = max(0, c - 32)
            else:
                for i, c in enumerate(wall_rgb):
                    base_wall[y, x, i] = c
    for i, c in enumerate(wall_rgb):
        n = rng.integers(-10, 10, size=(32, 32), dtype=np.int16)
        img[32:64, :, i] = np.clip(base_wall[:, :, i].astype(np.int16) + n, 0, 255).astype(np.uint8)
    # highlight top row
    for i, c in enumerate(wall_rgb):
        img[32, :, i] = min(255, c + 60)

    # Tile 2: Corridor floor (y 64-95)
    for i, c in enumerate(corridor_rgb):
        img[64:96, :, i] = noise(max(0, c - 8), -5, 5)
    # dot pattern
    for dy in range(4, 32, 8):
        for dx in range(4, 32, 8):
            for i, c in enumerate(corridor_rgb):
                img[64 + dy, dx, i] = max(0, c - 22)

    # Tile 3: Corridor wall / pillar (y 96-127)
    for i, c in enumerate(wall_rgb):
        img[96:128, :, i] = noise(max(0, c - 12), -6, 6)
    for i, c in enumerate(wall_rgb):
        img[96, :, i] = min(255, c + 40)

    Image.fromarray(img, 'RGBA').save(path)
    print(f"  Saved {path}")


print("Generating tilesets...")
make_tileset('game_demo/assets/textures/tileset_l1.png',
             floor_rgb=(52, 44, 32), wall_rgb=(92, 70, 48), corridor_rgb=(40, 35, 28))
make_tileset('game_demo/assets/textures/tileset_l2.png',
             floor_rgb=(30, 32, 50), wall_rgb=(60, 60, 90), corridor_rgb=(22, 24, 40))
make_tileset('game_demo/assets/textures/tileset_l3.png',
             floor_rgb=(48, 14, 14), wall_rgb=(90, 22, 22), corridor_rgb=(35, 8, 8))
print("Done.")
