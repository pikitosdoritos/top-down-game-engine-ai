#pragma once
#include "engine/Core/Types.hpp"
#include "engine/Components/VelocityComponent.hpp"
#include <vector>
#include <memory>

namespace engine {

class Entity;

// AABB-based collision detection and resolution.
// Entities must have both TransformComponent and ColliderComponent.
class CollisionSystem {
public:
    // Process all pairs in the list.
    // Resolves solid colliders (isTrigger=false) and fires callbacks.
    void process(std::vector<std::unique_ptr<Entity>>& entities);

private:
    // Returns overlap vector (push-out) if colliding, else {0,0}.
    static Vec2f getOverlap(const FloatRect& a, const FloatRect& b);

    // Resolve a solid collision: push the lighter entity out.
    static void resolve(Entity& a, Entity& b, Vec2f overlap);
};

} // namespace engine
