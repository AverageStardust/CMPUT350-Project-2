#pragma once

#include "MathUtil.h"
#include "SpriteManager.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace ECSEngine {

// The sprite rectangle is relative to the entity location in the world
// This is only defined if an entity also has a LocationComponent.
class SpriteComponent {
public:
    SpriteID spriteID;
    Point2D offset = {};
    bool worldSpace;  // If false, this sprite is drawn in screen space instead of world space
private:
};

}  // namespace ECSEngine
