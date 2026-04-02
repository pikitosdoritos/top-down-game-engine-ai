from PIL import Image, ImageDraw
import random
import os

img = Image.new('RGB', (32, 64), color=(30, 25, 45))
draw = ImageDraw.Draw(img)

# Floor tile (top 32x32)
for y in range(32):
    for x in range(32):
        if random.random() < 0.1:
            draw.point((x, y), fill=(40, 35, 55))
        elif random.random() < 0.05:
            draw.point((x, y), fill=(20, 15, 30))

# Wall tile (bottom 32x32)
draw.rectangle([0, 32, 31, 63], fill=(60, 50, 75))
# Mortar lines
for y in range(32, 64, 8):
    draw.line([(0, y), (31, y)], fill=(20, 15, 30))
    offset = 0 if (y - 32) % 16 == 0 else 16
    for x in range(offset, 32, 16):
        draw.line([(x, y), (x, y+7)], fill=(20, 15, 30))

# Highlight top edge of bricks
for y in range(32, 64, 8):
    draw.line([(0, y+1), (31, y+1)], fill=(80, 70, 95))
    offset = 0 if (y - 32) % 16 == 0 else 16
    for x in range(offset, 32, 16):
        draw.line([(x+1, y+1), (x+1, y+7)], fill=(80, 70, 95))

os.makedirs('game_demo/assets/textures', exist_ok=True)
img.save('game_demo/assets/textures/tileset.png')
print("Generated tileset.png")
