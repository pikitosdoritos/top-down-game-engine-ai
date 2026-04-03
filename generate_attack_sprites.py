"""
Generate sprite sheets for sword swing animation and bullet trail animation.
Outputs:
  game_demo/assets/textures/sword_swing.png  — 8 frames, each 64x64
  game_demo/assets/textures/bullet_anim.png  — 4 frames, each 32x32
"""

from PIL import Image, ImageDraw
import math
import os

OUT_DIR = "game_demo/assets/textures"
os.makedirs(OUT_DIR, exist_ok=True)

# ── Sword swing sheet ─────────────────────────────────────────────────────────
# 8 frames of a sword arc sweeping from -60° to +60° around the right axis.
# Each frame is 64×64 px, sheet is 512×64.

SWORD_FRAMES  = 8
SWORD_FRAME_W = 64
SWORD_FRAME_H = 64

sword_sheet = Image.new("RGBA", (SWORD_FRAME_W * SWORD_FRAMES, SWORD_FRAME_H), (0, 0, 0, 0))

for i in range(SWORD_FRAMES):
    frame = Image.new("RGBA", (SWORD_FRAME_W, SWORD_FRAME_H), (0, 0, 0, 0))
    draw  = ImageDraw.Draw(frame)

    t      = i / (SWORD_FRAMES - 1)          # 0 → 1
    angle  = math.radians(-60 + 120 * t)     # sweep -60° → +60°
    cx, cy = SWORD_FRAME_W // 2, SWORD_FRAME_H // 2

    # Alpha: bright in the middle of the swing, fades at ends
    alpha = int(255 * (1.0 - abs(t - 0.5) * 2.0) * 0.85 + 40)

    # Draw arc trail (3 overlapping lines of decreasing width)
    for width, color_alpha in [(7, alpha), (4, min(255, alpha + 60)), (2, 255)]:
        # Blade line from center outward
        blade_len = 26
        ex = cx + int(math.cos(angle) * blade_len)
        ey = cy + int(math.sin(angle) * blade_len)
        draw.line([(cx, cy), (ex, ey)],
                  fill=(220, 200, 80, color_alpha), width=width)

    # Bright tip dot
    tip_x = cx + int(math.cos(angle) * 26)
    tip_y = cy + int(math.sin(angle) * 26)
    r = 3
    draw.ellipse([tip_x - r, tip_y - r, tip_x + r, tip_y + r],
                 fill=(255, 240, 160, min(255, alpha + 80)))

    # Arc trail ghost (previous position, half alpha)
    if i > 0:
        prev_angle = math.radians(-60 + 120 * ((i - 1) / (SWORD_FRAMES - 1)))
        px = cx + int(math.cos(prev_angle) * 24)
        py = cy + int(math.sin(prev_angle) * 24)
        draw.line([(cx, cy), (px, py)],
                  fill=(255, 220, 100, alpha // 3), width=3)

    sword_sheet.paste(frame, (i * SWORD_FRAME_W, 0))

sword_path = os.path.join(OUT_DIR, "sword_swing.png")
sword_sheet.save(sword_path)
print(f"Saved {sword_path}  ({SWORD_FRAMES} frames, {SWORD_FRAME_W}x{SWORD_FRAME_H} each)")


# ── Bullet animation sheet ────────────────────────────────────────────────────
# 4 frames: glowing orb with a trailing tail that shrinks as it travels.
# Each frame is 32×32 px, sheet is 128×32.

BULLET_FRAMES  = 4
BULLET_FRAME_W = 32
BULLET_FRAME_H = 32

bullet_sheet = Image.new("RGBA", (BULLET_FRAME_W * BULLET_FRAMES, BULLET_FRAME_H), (0, 0, 0, 0))

for i in range(BULLET_FRAMES):
    frame = Image.new("RGBA", (BULLET_FRAME_W, BULLET_FRAME_H), (0, 0, 0, 0))
    draw  = ImageDraw.Draw(frame)

    t  = i / (BULLET_FRAMES - 1)   # 0 → 1  (spawn → near-death)
    cx = BULLET_FRAME_W  // 2
    cy = BULLET_FRAME_H  // 2

    # Fade out over lifetime
    life_alpha = int(255 * (1.0 - t * 0.7))

    # Tail — horizontal line to the left (bullet travels right by convention)
    tail_len = int(12 * (1.0 - t * 0.5))
    for j in range(tail_len):
        ta = int(life_alpha * (1.0 - j / tail_len) * 0.6)
        tx = cx - j - 4
        if 0 <= tx < BULLET_FRAME_W:
            draw.ellipse([tx - 1, cy - 1, tx + 1, cy + 1],
                         fill=(255, 140, 20, ta))

    # Outer glow
    glow_r = 7
    for g in range(glow_r, 0, -1):
        ga = int(life_alpha * (g / glow_r) * 0.35)
        draw.ellipse([cx - g, cy - g, cx + g, cy + g],
                     fill=(255, 100, 10, ga))

    # Core orb
    core_r = 4
    draw.ellipse([cx - core_r, cy - core_r, cx + core_r, cy + core_r],
                 fill=(255, 200, 80, life_alpha))

    # Bright centre highlight
    draw.ellipse([cx - 2, cy - 2, cx + 2, cy + 2],
                 fill=(255, 255, 200, min(255, life_alpha + 40)))

    bullet_sheet.paste(frame, (i * BULLET_FRAME_W, 0))

bullet_path = os.path.join(OUT_DIR, "bullet_anim.png")
bullet_sheet.save(bullet_path)
print(f"Saved {bullet_path}  ({BULLET_FRAMES} frames, {BULLET_FRAME_W}x{BULLET_FRAME_H} each)")

print("Done.")
