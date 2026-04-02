#include "engine/Collision/CollisionSystem.hpp"
#include "engine/Entity/Entity.hpp"
#include "engine/Components/TransformComponent.hpp"
#include "engine/Components/ColliderComponent.hpp"
#include "engine/Components/VelocityComponent.hpp"
#include <cmath>
#include <algorithm>

namespace engine {

void CollisionSystem::process(std::vector<std::unique_ptr<Entity>>& entities)
{
    std::vector<Entity*> collidables;
    collidables.reserve(entities.size());
    for (auto& e : entities) {
        if (e->active && !e->destroyed &&
            e->hasComponent<TransformComponent>() &&
            e->hasComponent<ColliderComponent>())
        {
            collidables.push_back(e.get());
        }
    }

    for (size_t i = 0; i < collidables.size(); ++i) {
        for (size_t j = i + 1; j < collidables.size(); ++j) {
            Entity* a = collidables[i];
            Entity* b = collidables[j];

            auto& ca = *a->getComponent<ColliderComponent>();
            auto& cb = *b->getComponent<ColliderComponent>();

            if (!(ca.layer & cb.mask) && !(cb.layer & ca.mask)) continue;

            auto& ta = *a->getComponent<TransformComponent>();
            auto& tb = *b->getComponent<TransformComponent>();

            FloatRect ra = ca.worldBounds(ta.position);
            FloatRect rb = cb.worldBounds(tb.position);

            Vec2f overlap = getOverlap(ra, rb);
            if (overlap.x == 0.f && overlap.y == 0.f) continue;

            if (ca.onCollision) ca.onCollision(*b);
            if (cb.onCollision) cb.onCollision(*a);

            if (!ca.isTrigger && !cb.isTrigger)
                resolve(*a, *b, overlap);
        }
    }
}

Vec2f CollisionSystem::getOverlap(const FloatRect& a, const FloatRect& b)
{
    // SFML 3: FloatRect uses .position and .size
    float ax1 = a.position.x,             ay1 = a.position.y;
    float ax2 = a.position.x + a.size.x,  ay2 = a.position.y + a.size.y;
    float bx1 = b.position.x,             by1 = b.position.y;
    float bx2 = b.position.x + b.size.x,  by2 = b.position.y + b.size.y;

    float ox = std::min(ax2, bx2) - std::max(ax1, bx1);
    float oy = std::min(ay2, by2) - std::max(ay1, by1);

    if (ox <= 0.f || oy <= 0.f) return {0.f, 0.f};
    return {ox, oy};
}

void CollisionSystem::resolve(Entity& a, Entity& b, Vec2f overlap)
{
    auto* ta = a.getComponent<TransformComponent>();
    auto* tb = b.getComponent<TransformComponent>();
    if (!ta || !tb) return;

    Vec2f push = (overlap.x < overlap.y)
        ? Vec2f{overlap.x, 0.f}
        : Vec2f{0.f, overlap.y};

    Vec2f diff = ta->position - tb->position;

    bool aStatic = !a.hasComponent<VelocityComponent>();
    bool bStatic = !b.hasComponent<VelocityComponent>();

    if (aStatic && bStatic) return;

    auto pushed = (diff.x >= 0.f) ? push : Vec2f{-push.x, push.y};

    if (aStatic) {
        tb->position -= pushed;
    } else if (bStatic) {
        ta->position += pushed;
    } else {
        ta->position += pushed * 0.5f;
        tb->position -= pushed * 0.5f;
    }
}

} // namespace engine
