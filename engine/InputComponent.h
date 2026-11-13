#pragma once

#include <limits.h>

#include <limits>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Keyboard.hpp>

namespace ECSEngine {

struct InputComponent {
    InputComponent(uint maxDashes = 1) : maxDashes(maxDashes) {}
    uint maxDashes;
    uint dashesLeft = 0;
    uint dashLockout = 0;
    uint jumpTime = 0;
    uint jumpBufferTime = 0;
    uint timeSinceGround = std::numeric_limits<uint>::max() / 2;
    uint timeSinceLeftWall = std::numeric_limits<uint>::max() / 2;
    uint timeSinceRightWall = std::numeric_limits<uint>::max() / 2;
};

}  // namespace ECSEngine
