#pragma once

#include <SFML/Audio.hpp>

#include <stdexcept>
#include "Storage.h"

namespace ECSEngine {

class SoundManager {
public:
    SoundManager() = default;
    ~SoundManager() = default;
    Ticket RegisterSound(const std::string& soundPath);
    sf::Sound& GetSound(Ticket soundName);
    void Play(Ticket soundName);

private:
    Storage<std::string> managedSounds;
};

}  // namespace ECSEngine
