"""Fast brute sprite chroma-key using NumPy — completes in < 1 second."""
import numpy as np
from PIL import Image
import os

src = r"C:\Users\Acer\.gemini\antigravity\brain\1b5e2025-6c3f-48cc-ad64-bb7a18f485b2\brute_sprite_1775220996191.png"
dest = "game_demo/assets/textures/brute.png"

if not os.path.exists(src):
    print(f"Source not found: {src}")
else:
    img = Image.open(src).convert("RGBA")
    arr = np.array(img, dtype=np.float32)  # H x W x 4

    r, g, b, a = arr[:,:,0], arr[:,:,1], arr[:,:,2], arr[:,:,3]

    # Green screen mask: green dominant, not grey
    green_mask = (g > 140) & (r < 120) & (b < 120)
    green_mask |= (g > r * 1.35) & (g > b * 1.35) & (g > 90)

    arr[green_mask, 3] = 0  # transparent
    out = Image.fromarray(arr.astype(np.uint8), 'RGBA')
    out.thumbnail((52, 52), Image.Resampling.LANCZOS)
    os.makedirs(os.path.dirname(dest), exist_ok=True)
    out.save(dest)
    print(f"Saved {dest}")
