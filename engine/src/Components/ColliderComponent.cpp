#include "engine/Components/ColliderComponent.hpp"

namespace engine {

ColliderComponent::ColliderComponent(Vec2f size, Vec2f offset)
    : size(size), offset(offset)
{}

FloatRect ColliderComponent::worldBounds(const Vec2f& ownerPos) const
{
    // SFML 3: FloatRect({position}, {size})
    return FloatRect(
        {ownerPos.x + offset.x - size.x * 0.5f,
         ownerPos.y + offset.y - size.y * 0.5f},
        {size.x, size.y}
    );
}

} // namespace engine
