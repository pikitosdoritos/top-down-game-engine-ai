#pragma once
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/Music.hpp>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

namespace engine {

class AudioManager {
public:
    // Load a short sound effect into cache
    bool loadSound(const std::string& name, const std::string& path);

    // Play a cached sound (non-blocking)
    void playSound(const std::string& name, float volume = 100.f);

    // Stream background music (one track at a time)
    bool playMusic(const std::string& path, bool loop = true, float volume = 50.f);
    void stopMusic();
    void setMusicVolume(float volume);

    void setMasterVolume(float volume); // 0–100
    float masterVolume() const { return m_masterVolume; }

    // Prune finished Sound instances each frame
    void update();

private:
    std::unordered_map<std::string, sf::SoundBuffer> m_buffers;
    std::vector<sf::Sound>                           m_activeSounds;
    sf::Music                                        m_music;
    float                                            m_masterVolume = 100.f;
};

} // namespace engine
