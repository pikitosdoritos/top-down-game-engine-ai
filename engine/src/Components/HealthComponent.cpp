#include "engine/Components/HealthComponent.hpp"
#include <algorithm>

namespace engine {

void HealthComponent::takeDamage(float amount)
{
    if (amount <= 0.f || currentHealth <= 0.f) return;
    currentHealth = std::max(0.f, currentHealth - amount);
    if (onDamaged) onDamaged(amount);
    if (currentHealth <= 0.f && onDeath) onDeath();
}

void HealthComponent::heal(float amount)
{
    currentHealth = std::min(maxHealth, currentHealth + amount);
}

} // namespace engine
