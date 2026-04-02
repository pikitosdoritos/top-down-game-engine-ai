#include "engine/Renderer/Renderer.hpp"

namespace engine {

void Renderer::init(sf::RenderWindow& window)
{
    m_window = &window;
}

void Renderer::beginFrame(sf::Color clearColor)
{
    m_window->clear(clearColor);
}

void Renderer::endFrame()
{
    m_window->display();
}

void Renderer::setWorldView(const sf::View& view)
{
    m_window->setView(view);
}

void Renderer::setUIView()
{
    m_window->setView(m_window->getDefaultView());
}

void Renderer::resetView()
{
    m_window->setView(m_window->getDefaultView());
}

} // namespace engine
