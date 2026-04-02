#include "engine/Audio/AudioManager.hpp"
#include <algorithm>

namespace engine {

bool AudioManager::loadSound(const std::string& name, const std::string& path)
{
    sf::SoundBuffer buf;
    if (!buf.loadFromFile(path)) return false;
    m_buffers[name] = std::move(buf);
    return true;
}

void AudioManager::playSound(const std::string& name, float volume)
{
    auto it = m_buffers.find(name);
    if (it == m_buffers.end()) return;

    m_activeSounds.emplace_back(it->second);
    auto& s = m_activeSounds.back();
    s.setVolume(volume * m_masterVolume / 100.f);
    s.play();
}

bool AudioManager::playMusic(const std::string& path, bool loop, float volume)
{
    if (!m_music.openFromFile(path)) return false;
    m_music.setLooping(loop);
    m_music.setVolume(volume * m_masterVolume / 100.f);
    m_music.play();
    return true;
}

void AudioManager::stopMusic()
{
    m_music.stop();
}

void AudioManager::setMusicVolume(float volume)
{
    m_music.setVolume(volume * m_masterVolume / 100.f);
}

void AudioManager::setMasterVolume(float volume)
{
    m_masterVolume = volume;
}

void AudioManager::update()
{
    // SFML 3: Status is an enum class — sf::SoundSource::Status::Stopped
    m_activeSounds.erase(
        std::remove_if(m_activeSounds.begin(), m_activeSounds.end(),
            [](const sf::Sound& s) {
                return s.getStatus() == sf::SoundSource::Status::Stopped;
            }),
        m_activeSounds.end()
    );
}

} // namespace engine
