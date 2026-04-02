#include "engine/Components/AnimationComponent.hpp"
#include "engine/Components/SpriteComponent.hpp"
#include "engine/Entity/Entity.hpp"
#include "engine/Core/GameEngine.hpp"

namespace engine {

void AnimationComponent::addAnimation(const Animation& anim)
{
    m_animations[anim.name] = anim;
}

void AnimationComponent::play(const std::string& name, bool forceRestart)
{
    if (m_current == name && !forceRestart) return;
    m_current    = name;
    m_frameIndex = 0;
    m_elapsed    = 0.f;
    m_finished   = false;
}

void AnimationComponent::stop()
{
    m_current  = "";
    m_finished = true;
}

void AnimationComponent::update(Entity& owner, GameEngine& /*engine*/, float dt)
{
    if (m_current.empty()) return;
    auto it = m_animations.find(m_current);
    if (it == m_animations.end()) return;

    const Animation& anim = it->second;
    if (anim.frames.empty()) return;

    m_elapsed += dt;
    const AnimationFrame& frame = anim.frames[m_frameIndex];

    if (m_elapsed >= frame.duration) {
        m_elapsed -= frame.duration;
        m_frameIndex++;

        if (m_frameIndex >= static_cast<int>(anim.frames.size())) {
            if (anim.looping) {
                m_frameIndex = 0;
            } else {
                m_frameIndex = static_cast<int>(anim.frames.size()) - 1;
                m_finished   = true;
            }
        }
    }

    // Push rect to SpriteComponent
    if (auto* sc = owner.getComponent<SpriteComponent>()) {
        sc->setTextureRect(anim.frames[m_frameIndex].rect);
    }
}

} // namespace engine
