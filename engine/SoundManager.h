#pragma once

#include <utility>
#include <vector>

#include "SFML/Audio/Sound.hpp"
#include "Storage.h"
#include <SFML/Audio.hpp>

namespace ECSEngine {

using SoundID = Ticket;

class SoundManager {
public:
    SoundManager() = default;
    ~SoundManager() = default;
    SoundID RegisterSound(const std::string& soundPath);

    /*
     * The callee is responsible to keep the sound in memory until it's finished, and then free it.
     * If you don't want this, use Play( ) instead
     */
    sf::Sound* GetSound(SoundID soundName);
    void Play(SoundID soundName, float volume = 1);

private:
    Storage<sf::SoundBuffer> managedBuffers;
    Storage<sf::Sound*> playingSounds;

    void Cleanup();

    template <typename... Any>
    friend class ECSEngine;
};

}  // namespace ECSEngine
