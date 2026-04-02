#pragma once
#include "engine/Entity/Component.hpp"
#include "engine/Core/Types.hpp"
#include <functional>

namespace engine {

class Entity;

class ColliderComponent : public Component {
public:
    // offset is relative to entity's TransformComponent position
    ColliderComponent(Vec2f size, Vec2f offset = {0.f, 0.f});

    // Returns the AABB in world space
    FloatRect worldBounds(const Vec2f& ownerPos) const;

    Vec2f size   {0.f, 0.f};
    Vec2f offset {0.f, 0.f};

    CollisionLayer layer  = CollisionLayer::None;
    CollisionLayer mask   = CollisionLayer::None; // layers this collides with

    bool isTrigger = false; // trigger = no resolution, just callback

    // Called when a collision is detected (both sides get called)
    std::function<void(Entity& other)> onCollision;
};

} // namespace engine
