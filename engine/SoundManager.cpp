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

sf::Sound* SoundManager::GetSound(SoundID soundID) {
    sf::SoundBuffer& buffer = managedBuffers.Get(soundID);
    return new sf::Sound(buffer);
}

void SoundManager::Play(SoundID soundID, float volume) {
    Ticket playingSound = playingSounds.Store(GetSound(soundID));
    sf::Sound* sound = playingSounds.Get(playingSound);
    sound->setVolume(volume * 100);
    sound->play();
}

void SoundManager::Cleanup() {
    // remove finished sounds created by Play( )
    // called by ECSEngine.Run( )
    for (auto iter = playingSounds.begin(); iter != playingSounds.end(); iter++) {
        sf::Sound* sound = *iter;

        if (sound->getStatus() == sf::Sound::Status::Stopped) {
            delete sound;
            playingSounds.Remove(iter.GetTicket());
        }
    }
}

}  // namespace ECSEngine
