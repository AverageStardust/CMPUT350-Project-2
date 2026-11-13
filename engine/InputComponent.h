#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <limits.h>

namespace ECSEngine {

struct InputComponent {
    InputComponent() : dashesLeft(0) {}
    uint dashesLeft;
    uint airTime = std::numeric_limits<uint>::max() / 2;
};

}  // namespace ECSEngine
