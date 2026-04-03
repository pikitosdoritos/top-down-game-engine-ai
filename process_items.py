import numpy as np
from PIL import Image
import os

def chroma_key(src, dest, size):
    if not os.path.exists(src):
        print(f"Source not found: {src}")
        return
    img = Image.open(src).convert("RGBA")
    arr = np.array(img, dtype=np.float32)

    r, g, b, a = arr[:,:,0], arr[:,:,1], arr[:,:,2], arr[:,:,3]
    # Green screen mask: green dominant
    green_mask = (g > 140) & (r < 120) & (b < 120)
    green_mask |= (g > r * 1.35) & (g > b * 1.35) & (g > 90)

    arr[green_mask, 3] = 0
    out = Image.fromarray(arr.astype(np.uint8), 'RGBA')
    out.thumbnail(size, Image.Resampling.LANCZOS)
    os.makedirs(os.path.dirname(dest), exist_ok=True)
    out.save(dest)
    print(f"Saved {dest}")

chroma_key(r"C:\Users\Acer\.gemini\antigravity\brain\1b5e2025-6c3f-48cc-ad64-bb7a18f485b2\weapon_sword_1775224227867.png", "game_demo/assets/textures/sword.png", (32, 32))
chroma_key(r"C:\Users\Acer\.gemini\antigravity\brain\1b5e2025-6c3f-48cc-ad64-bb7a18f485b2\bullet_magic_1775224240247.png", "game_demo/assets/textures/bullet.png", (16, 16))
