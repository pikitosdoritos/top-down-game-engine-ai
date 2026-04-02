#include "engine/Scene/SceneManager.hpp"
#include "engine/Core/GameEngine.hpp"

namespace engine {

void SceneManager::registerScene(SceneID id,
    std::function<std::unique_ptr<IScene>()> factory)
{
    m_factories[id] = std::move(factory);
}

void SceneManager::switchTo(SceneID id)
{
    m_pending    = id;
    m_hasPending = true;
}

void SceneManager::applyPendingSwitch(GameEngine& engine)
{
    if (!m_hasPending) return;
    m_hasPending = false;

    if (m_current)
        m_current->onExit(engine);

    auto it = m_factories.find(m_pending);
    if (it != m_factories.end())
        m_current = it->second();
    else
        m_current = nullptr;

    if (m_current)
        m_current->onEnter(engine);
}

void SceneManager::handleEvent(GameEngine& engine, const sf::Event& event)
{
    if (m_current)
        m_current->handleEvent(engine, event);
}

void SceneManager::update(GameEngine& engine, float dt)
{
    if (m_current)
        m_current->update(engine, dt);
}

void SceneManager::render(GameEngine& engine)
{
    if (m_current)
        m_current->render(engine);
}

} // namespace engine
