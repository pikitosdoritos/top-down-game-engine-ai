#pragma once
#include "engine/Entity/Component.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <vector>
#include <string>
#include <unordered_map>

namespace engine {

struct AnimationFrame {
    sf::IntRect rect;        // sub-rect in the sprite sheet
    float       duration;    // seconds to hold this frame
};

struct Animation {
    std::string               name;
    std::vector<AnimationFrame> frames;
    bool                      looping = true;
};

class SpriteComponent;

class AnimationComponent : public Component {
public:
    void addAnimation(const Animation& anim);
    void play(const std::string& name, bool forceRestart = false);
    void stop();

    void update(Entity& owner, GameEngine& engine, float dt) override;

    const std::string& currentAnimation() const { return m_current; }
    bool isFinished() const { return m_finished; }

private:
    std::unordered_map<std::string, Animation> m_animations;
    std::string m_current;
    int         m_frameIndex  = 0;
    float       m_elapsed     = 0.f;
    bool        m_finished    = false;
};

} // namespace engine
