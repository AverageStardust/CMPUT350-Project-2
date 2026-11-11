#pragma once

#include "Entity.h"
#include "MathUtil.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace ECSEngine {

struct CameraComponent {
    Point2D position;  // [world coordinates] position in world coordinates
    float worldUnitsPerPixel;
};

struct CameraFollower {
    EntityID entityID;  // The entity that this camera follows
    Rect hardExclusion;  // [screen coordinates] Area where the camera is not allowed to go
                         // (negative of this rect)
    Rect softExclusion;  // [screen coordinates] Area where the camera is allowed to go (but will
                         // scroll out)
    float speed;
};

struct CameraShake {
    int frameLimit;
    Point2D offset;
};

}  // namespace ECSEngine
