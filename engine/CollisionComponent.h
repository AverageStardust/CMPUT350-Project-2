#pragma once

#include <bitset>
#include <unordered_map>

#include "MathUtil.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace ECSEngine {

struct CollisionComponent {
    uint8_t isCollectable : 1 = false;
    uint8_t isStatic : 1 = false;
    uint8_t doesDamage : 1 = false;
    uint8_t onGround : 1 = false;
    uint8_t onCeiling : 1 = false;
    uint8_t onLeftWall : 1 = false;
    uint8_t onRightWall : 1 = false;
    Rect localBounds = {};
    Rect worldBounds = {}; // Previous location
    Point2D lastVelocity = {};
};

}  // namespace ECSEngine
