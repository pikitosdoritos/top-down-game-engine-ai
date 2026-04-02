#pragma once
#include "engine/Entity/Component.hpp"
#include "engine/Core/Types.hpp"

namespace engine {

class TransformComponent : public Component {
public:
    TransformComponent() = default;
    explicit TransformComponent(Vec2f pos, float rot = 0.f, Vec2f scale = {1.f, 1.f})
        : position(pos), rotation(rot), scale(scale) {}

    Vec2f position {0.f, 0.f};
    float rotation {0.f};          // degrees
    Vec2f scale    {1.f, 1.f};
};

} // namespace engine
