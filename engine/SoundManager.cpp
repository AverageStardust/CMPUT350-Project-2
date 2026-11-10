#include "SoundManager.h"

namespace ECSEngine {

Ticket SoundManager::RegisterSound(const std::string& soundPath) {
    return managedSounds.Store(soundPath);
}

sf::Sound& SoundManager::GetSound(Ticket soundName) {
    std::string& soundPath = managedSounds.Get(soundName);
    static sf::SoundBuffer dummyBuffer;
    if (!dummyBuffer.loadFromFile(soundPath)) {
        throw std::runtime_error("Could not load sound");
    }
    static sf::Sound dummy(dummyBuffer);
    return dummy;
}

void SoundManager::Play(Ticket soundName) {
    GetSound(soundName).play();
}

}  // namespace ECSEngine
