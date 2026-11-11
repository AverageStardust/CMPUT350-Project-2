#pragma once

#include <bitset>

#include "Entity.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Keyboard.hpp>

namespace ECSEngine {

struct InputComponent {
    InputComponent() : dashesLeft(0) {}
    int dashesLeft;
};

}  // namespace ECSEngine
