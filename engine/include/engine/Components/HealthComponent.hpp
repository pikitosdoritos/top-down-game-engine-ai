#pragma once
#include "engine/Entity/Component.hpp"
#include <functional>

namespace engine {

class HealthComponent : public Component {
public:
    explicit HealthComponent(float maxHp)
        : maxHealth(maxHp), currentHealth(maxHp) {}

    void takeDamage(float amount);
    void heal(float amount);

    bool isAlive()  const { return currentHealth > 0.f; }
    bool isDead()   const { return !isAlive(); }
    float ratio()   const { return currentHealth / maxHealth; }

    float maxHealth     = 100.f;
    float currentHealth = 100.f;

    // Optional callbacks
    std::function<void(float /*damage*/)> onDamaged;
    std::function<void()>                onDeath;
};

} // namespace engine
