#pragma once

#include <limits.h>

#include <limits>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Keyboard.hpp>

namespace ECSEngine {

struct InputComponent {
    InputComponent() : dashesLeft(0) {}
    uint dashesLeft;
    uint jumpTime = 0;
    uint jumpBufferTime = 0;
    uint timeSinceGround = std::numeric_limits<uint>::max() / 2;
    uint timeSinceLeftWall = std::numeric_limits<uint>::max() / 2;
    uint timeSinceRightWall = std::numeric_limits<uint>::max() / 2;
};

}  // namespace ECSEngine
