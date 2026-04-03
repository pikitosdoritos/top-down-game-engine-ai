#pragma once
#include "engine/Entity/Component.hpp"
#include "engine/Core/Types.hpp"
#include <functional>

namespace engine {

class Entity;
class GameEngine;

// Simple state-machine AI for enemies.
enum class AIState {
    Idle,
    Chase,
    Attack,
    Dead,
};

class AIComponent : public Component {
public:
    // behaviourUpdate is called every frame in addition to built-in state logic.
    // Assign custom logic per enemy type here.
    std::function<void(Entity& self, GameEngine& engine, float dt)> behaviourUpdate;

    // Optional line-of-sight check. If set, Chase only activates when this returns true.
    // Signature: (from, to) -> bool (true = clear line of sight)
    std::function<bool(Vec2f from, Vec2f to)> hasLineOfSight;

    void update(Entity& owner, GameEngine& engine, float dt) override;

    AIState state         = AIState::Idle;
    float   detectionRange = 400.f;   // px — switches Idle→Chase
    float   attackRange    = 50.f;    // px — switches Chase→Attack
    float   moveSpeed      = 100.f;   // px/s
    float   attackDamage   = 10.f;
    float   attackCooldown = 1.0f;    // seconds between attacks
    float   attackTimer    = 0.f;

    // Set by GameScene each frame so AI knows where to go
    Vec2f targetPosition {0.f, 0.f};
};

} // namespace engine
