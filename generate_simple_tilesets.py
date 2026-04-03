from PIL import Image, ImageDraw
import random
import os

def create_simple_tileset(path, floor_color, wall_color):
    # 32x64 tileset (2 tiles stacked)
    # Tile 0: Floor
    # Tile 1: Wall
    tile_size = 32
    img = Image.new('RGBA', (tile_size, tile_size * 2), (0, 0, 0, 255))
    draw = ImageDraw.Draw(img)
    
    # Floor tile
    for y in range(tile_size):
        for x in range(tile_size):
            # noise
            n = random.randint(-5, 5)
            c = tuple(max(0, min(255, val + n)) for val in floor_color)
            draw.point((x, y), fill=c)
            
    # Wall tile
    wall_y_start = tile_size
    for y in range(tile_size):
        for x in range(tile_size):
            # brick pattern logic
            if y % 8 == 0 or (x % 16 == 0 and (y // 8) % 2 == 0) or ((x + 8) % 16 == 0 and (y // 8) % 2 != 0):
                c = tuple(max(0, min(255, val - 20)) for val in wall_color)
            else:
                n = random.randint(-10, 10)
                c = tuple(max(0, min(255, val + n)) for val in wall_color)
            draw.point((x, wall_y_start + y), fill=c)

    os.makedirs(os.path.dirname(path), exist_ok=True)
    img.save(path)
    print(f"Tileset saved to {path}")

# Level 1: Warm earth
create_simple_tileset('game_demo/assets/textures/tileset_l1.png', (52, 44, 32), (92, 70, 48))
# Level 2: Cold crypt
create_simple_tileset('game_demo/assets/textures/tileset_l2.png', (30, 32, 50), (60, 60, 90))
# Level 3: Hellish abyss
create_simple_tileset('game_demo/assets/textures/tileset_l3.png', (48, 14, 14), (90, 22, 22))
