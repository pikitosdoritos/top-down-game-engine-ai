#pragma once
#include "engine/Entity/Component.hpp"
#include "engine/Core/Types.hpp"

namespace engine {

// Stores current velocity; movement systems read/write this.
class VelocityComponent : public Component {
public:
    VelocityComponent() = default;
    explicit VelocityComponent(Vec2f v) : velocity(v) {}

    Vec2f velocity {0.f, 0.f};
    float maxSpeed {300.f};  // pixels per second cap (0 = no cap)
};

} // namespace engine
