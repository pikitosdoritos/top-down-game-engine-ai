from PIL import Image, ImageDraw, ImageFilter
import os, random

os.makedirs('game_demo/assets/textures', exist_ok=True)

def make_tileset(path, theme):
    """
    Creates a 32x128 tileset PNG with 4 tiles stacked vertically:
      Row 0 (y=  0): floor
      Row 1 (y= 32): wall
      Row 2 (y= 64): corridor floor
      Row 3 (y= 96): corridor wall / pillar
    """
    W, H = 32, 128
    img = Image.new('RGBA', (W, H), (0, 0, 0, 255))
    draw = ImageDraw.Draw(img)

    fr, fg, fb = theme['floor']
    wr, wg, wb = theme['wall']
    cr, cg, cb = theme['corridor']

    # ── TILE 0: Room floor ────────────────────────────────────────────────
    for y in range(32):
        for x in range(32):
            noise = random.randint(-8, 8)
            col = (max(0,min(255,fr+noise)), max(0,min(255,fg+noise)), max(0,min(255,fb+noise)), 255)
            draw.point((x, y), fill=col)
    # subtle grid crack lines
    for i in range(0, 32, 8):
        draw.line([(i, 0), (i, 31)], fill=(max(0,fr-18), max(0,fg-18), max(0,fb-18), 255))
        draw.line([(0, i), (31, i)], fill=(max(0,fr-18), max(0,fg-18), max(0,fb-18), 255))

    # ── TILE 1: Wall ─────────────────────────────────────────────────────
    base_y = 32
    for y in range(32):
        for x in range(32):
            # brick pattern
            row = y // 8
            offset = 0 if row % 2 == 0 else 16
            is_mortar_h = (y % 8 == 0)
            is_mortar_v = ((x + offset) % 16 == 0)
            if is_mortar_h or is_mortar_v:
                col = (max(0,wr-30), max(0,wg-30), max(0,wb-30), 255)
            else:
                noise = random.randint(-10, 10)
                col = (max(0,min(255,wr+noise)), max(0,min(255,wg+noise)), max(0,min(255,wb+noise)), 255)
            draw.point((x, base_y + y), fill=col)
    # top highlight
    draw.line([(0, base_y), (31, base_y)], fill=(min(255,wr+60), min(255,wg+60), min(255,wb+60), 255))

    # ── TILE 2: Corridor floor ────────────────────────────────────────────
    base_y = 64
    cdr, cdg, cdb = max(0,cr-10), max(0,cg-10), max(0,cb-10)
    for y in range(32):
        for x in range(32):
            noise = random.randint(-5, 5)
            col = (max(0,min(255,cdr+noise)), max(0,min(255,cdg+noise)), max(0,min(255,cdb+noise)), 255)
            draw.point((x, base_y + y), fill=col)
    # subtle dotted pattern
    for i in range(4, 32, 8):
        for j in range(4, 32, 8):
            draw.point((i, base_y+j), fill=(max(0,cdr-20), max(0,cdg-20), max(0,cdb-20), 255))

    # ── TILE 3: Corridor wall / pillar ─────────────────────────────────────
    base_y = 96
    for y in range(32):
        for x in range(32):
            noise = random.randint(-6, 6)
            col = (max(0,min(255,wr+noise-10)), max(0,min(255,wg+noise-10)), max(0,min(255,wb+noise-10)), 255)
            draw.point((x, base_y + y), fill=col)
    draw.line([(0, base_y), (31, base_y)], fill=(min(255,wr+40), min(255,wg+40), min(255,wb+40), 255))

    img.save(path)
    print(f"Saved {path}")


random.seed(42)
make_tileset('game_demo/assets/textures/tileset_l1.png', {
    'floor':    (52, 44, 32),   # warm stone
    'wall':     (92, 70, 48),   # earthy brick
    'corridor': (40, 35, 28),   # dark passage
})

random.seed(13)
make_tileset('game_demo/assets/textures/tileset_l2.png', {
    'floor':    (30, 32, 50),   # cold slate
    'wall':     (60, 60, 90),   # blue-grey stone
    'corridor': (22, 24, 40),   # deep crypt
})

random.seed(77)
make_tileset('game_demo/assets/textures/tileset_l3.png', {
    'floor':    (48, 14, 14),   # blood stone
    'wall':     (90, 22, 22),   # dark red brick
    'corridor': (35, 8,  8),    # abyss passage
})

print("All tilesets generated.")
