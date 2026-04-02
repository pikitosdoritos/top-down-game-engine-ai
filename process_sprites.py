from PIL import Image
import os

def process_image(src_path, dest_path, target_size=(32, 32)):
    if not os.path.exists(src_path):
        print(f"Skipping {src_path}")
        return
        
    img = Image.open(src_path).convert("RGBA")
    data = img.getdata()
    
    # Simple chroma key: remove pure/near green
    new_data = []
    for item in data:
        r, g, b, a = item
        # If green is dominant, make transparent
        if g > 150 and r < 100 and b < 100:
            new_data.append((255, 255, 255, 0))
        elif g > r * 1.5 and g > b * 1.5:
             new_data.append((255, 255, 255, 0))
        else:
            new_data.append(item)
            
    img.putdata(new_data)
    
    # Resize keeping aspect ratio
    img.thumbnail(target_size, Image.Resampling.LANCZOS)
    
    # Save
    os.makedirs(os.path.dirname(dest_path), exist_ok=True)
    img.save(dest_path)
    print(f"Saved {dest_path}")

player_src = r"C:\Users\Acer\.gemini\antigravity\brain\1b5e2025-6c3f-48cc-ad64-bb7a18f485b2\player_sprite_1775138233167.png"
enemy_src = r"C:\Users\Acer\.gemini\antigravity\brain\1b5e2025-6c3f-48cc-ad64-bb7a18f485b2\enemy_sprite_1775138247313.png"

process_image(player_src, "game_demo/assets/textures/player.png", (48, 48))
process_image(enemy_src, "game_demo/assets/textures/enemy.png", (48, 48))
