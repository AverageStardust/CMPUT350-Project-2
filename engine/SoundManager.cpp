#include "SoundManager.h"

#include <iostream>

#include "SFML/Audio/Sound.hpp"
#include "WindowManager.h"

namespace ECSEngine {

SoundID SoundManager::RegisterSound(const std::string& soundPath) {
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(soundPath)) {
        throw std::runtime_error("Could not load sound");
    }

    return (SoundID)managedBuffers.Store(buffer);
}

sf::Sound SoundManager::GetSound(SoundID soundID) {
    sf::SoundBuffer& buffer = managedBuffers.Get(soundID);
    return sf::Sound(buffer);
}

void SoundManager::Play(SoundID soundID, float volume) {
    playingSounds.push_back({GetSound(soundID), 0});
    sf::Sound& sound = playingSounds.back().first;
    sound.setVolume(volume * 100);
    sound.play();
}

void SoundManager::Cleanup() {
    // remove finished sounds created by Play( )
    // called by ECSEngine.Run( )
    for (auto& pair : playingSounds) {
        if (pair.first.getStatus() == sf::Sound::Status::Stopped) {
            pair.second += 1.0f / FRAMES_PER_SECOND;
        }
    }

    // remove when not playing for 3 second to prevent clipping
    playingSounds.erase(std::remove_if(playingSounds.begin(), playingSounds.end(),
                                       [](const auto& pair) {
                                           return pair.second > 3;
                                       }),
                        playingSounds.end());
}

}  // namespace ECSEngine
