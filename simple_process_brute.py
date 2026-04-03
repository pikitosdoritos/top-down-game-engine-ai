from PIL import Image
import os

src = r"C:\Users\Acer\.gemini\antigravity\brain\1b5e2025-6c3f-48cc-ad64-bb7a18f485b2\brute_sprite_1775220996191.png"
dest = "game_demo/assets/textures/brute.png"

if not os.path.exists(src):
    print("Source not found")
else:
    img = Image.open(src).convert("RGBA")
    pixels = img.getdata()
    newData = []
    
    for r, g, b, a in pixels:
        # chroma key for #00FF00
        if g > 130 and r < 120 and b < 120:
            newData.append((255, 255, 255, 0))
        else:
            newData.append((r, g, b, a))
            
    img.putdata(newData)
    img = img.resize((52, 52), Image.Resampling.LANCZOS)
    os.makedirs(os.path.dirname(dest), exist_ok=True)
    img.save(dest)
    print("Brute sprite processed")
