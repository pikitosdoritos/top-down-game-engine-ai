#pragma once
#include "engine/Entity/Component.hpp"
#include "engine/Core/Types.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <string>

namespace engine {

class GameEngine;

class Entity {
public:
    explicit Entity(std::string tag = "entity");
    virtual ~Entity() = default;

    Entity(const Entity&)            = delete;
    Entity& operator=(const Entity&) = delete;
    Entity(Entity&&)                 = default;
    Entity& operator=(Entity&&)      = default;

    // --- Component management ---
    template<typename T, typename... Args>
    T& addComponent(Args&&... args) {
        static_assert(std::is_base_of_v<Component, T>);
        auto comp = std::make_unique<T>(std::forward<Args>(args)...);
        T* ptr = comp.get();
        comp->onAttach(*this);
        m_components[std::type_index(typeid(T))] = std::move(comp);
        return *ptr;
    }

    template<typename T>
    T* getComponent() {
        auto it = m_components.find(std::type_index(typeid(T)));
        if (it == m_components.end()) return nullptr;
        return static_cast<T*>(it->second.get());
    }

    template<typename T>
    const T* getComponent() const {
        auto it = m_components.find(std::type_index(typeid(T)));
        if (it == m_components.end()) return nullptr;
        return static_cast<const T*>(it->second.get());
    }

    template<typename T>
    bool hasComponent() const {
        return m_components.count(std::type_index(typeid(T))) > 0;
    }

    template<typename T>
    void removeComponent() {
        m_components.erase(std::type_index(typeid(T)));
    }

    virtual void update(GameEngine& engine, float dt);
    virtual void render(sf::RenderWindow& window) { (void)window; }

    const std::string& tag() const { return m_tag; }

    bool  active    = true;
    bool  destroyed = false;

protected:
    std::string m_tag;
    std::unordered_map<std::type_index, std::unique_ptr<Component>> m_components;
};

} // namespace engine
