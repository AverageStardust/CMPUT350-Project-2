#include "SoundManager.h"
#include <iostream>

#include "SFML/Audio/Sound.hpp"

namespace ECSEngine {

SoundID SoundManager::RegisterSound(const std::string& soundPath) {
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(soundPath)) {
        throw std::runtime_error("Could not load sound");
    }

    return (SoundID)managedSounds.Store(buffer);
}

sf::Sound SoundManager::GetSound(SoundID soundID) {
    sf::SoundBuffer& buffer = managedSounds.Get(soundID);
    return sf::Sound(buffer);
}

void SoundManager::Play(SoundID soundID) {
    playingSounds.push_back(GetSound(soundID));
    playingSounds.back().play();
}

void SoundManager::Cleanup() {
    // remove finished sounds created by Play( )
    // called by ECSEngine.Run( )
    playingSounds.erase(std::remove_if(playingSounds.begin(), playingSounds.end(),
                                       [](const sf::Sound& sound) {
                                           return sound.getStatus() == sf::Sound::Status::Stopped;
                                       }),
                        playingSounds.end());
}

}  // namespace ECSEngine
