from PIL import Image
import os

def chroma_key(src, dest, size=(48, 48)):
    if not os.path.exists(src):
        print(f"Not found: {src}")
        return
    img = Image.open(src).convert("RGBA")
    pixels = img.getdata()
    out = []
    for r, g, b, a in pixels:
        # Remove green screen
        if g > 140 and r < 110 and b < 110:
            out.append((255, 255, 255, 0))
        elif g > r * 1.4 and g > b * 1.4 and g > 100:
            out.append((255, 255, 255, 0))
        else:
            out.append((r, g, b, a))
    img.putdata(out)
    img.thumbnail(size, Image.Resampling.LANCZOS)
    os.makedirs(os.path.dirname(dest), exist_ok=True)
    img.save(dest)
    print(f"Processed -> {dest}")

chroma_key(
    r"C:\Users\Acer\.gemini\antigravity\brain\1b5e2025-6c3f-48cc-ad64-bb7a18f485b2\brute_sprite_1775220996191.png",
    "game_demo/assets/textures/brute.png",
    size=(52, 52)
)
