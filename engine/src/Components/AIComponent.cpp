#include "engine/Components/AIComponent.hpp"
#include "engine/Entity/Entity.hpp"
#include "engine/Components/TransformComponent.hpp"
#include "engine/Components/VelocityComponent.hpp"
#include "engine/Components/HealthComponent.hpp"
#include "engine/Core/GameEngine.hpp"
#include <cmath>

namespace engine {

static float length(Vec2f v) {
    return std::sqrt(v.x * v.x + v.y * v.y);
}

void AIComponent::update(Entity& owner, GameEngine& engine, float dt)
{
    // Check if dead
    if (auto* hp = owner.getComponent<HealthComponent>()) {
        if (hp->isDead()) {
            state = AIState::Dead;
        }
    }
    if (state == AIState::Dead) return;

    auto* tf = owner.getComponent<TransformComponent>();
    auto* vel = owner.getComponent<VelocityComponent>();
    if (!tf) return;

    Vec2f diff = targetPosition - tf->position;
    float dist = length(diff);

    // State transitions
    if (dist <= attackRange)
        state = AIState::Attack;
    else if (dist <= detectionRange)
        state = AIState::Chase;
    else
        state = AIState::Idle;

    // Behaviour
    if (state == AIState::Chase && vel) {
        Vec2f dir = (dist > 0.f) ? Vec2f{diff.x / dist, diff.y / dist} : Vec2f{0.f, 0.f};
        vel->velocity = dir * moveSpeed;
    } else if (state == AIState::Idle && vel) {
        vel->velocity = {0.f, 0.f};
    } else if (state == AIState::Attack && vel) {
        vel->velocity = {0.f, 0.f};
        attackTimer -= dt;
    }

    // Custom per-entity behaviour
    if (behaviourUpdate) behaviourUpdate(owner, engine, dt);
}

} // namespace engine
