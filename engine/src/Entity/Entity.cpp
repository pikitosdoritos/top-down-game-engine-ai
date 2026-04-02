#include "engine/Entity/Entity.hpp"
#include "engine/Core/GameEngine.hpp"

namespace engine {

Entity::Entity(std::string tag) : m_tag(std::move(tag)) {}

void Entity::update(GameEngine& engine, float dt)
{
    if (!active) return;
    for (auto& [type, comp] : m_components) {
        if (comp->enabled)
            comp->update(*this, engine, dt);
    }
}

} // namespace engine
