#pragma once

#include "Entity.h"
#include "MathUtil.h"
#include "SpriteManager.h"

namespace ECSEngine {

// The sprite rectangle is relative to the entity location in the world
// This is only defined if an entity also has a LocationComponent.
class ScoreComponent {
public:
    int score;
    Point2D offset = {8, 8};
    float spacing = 38;
    SpriteID digitSpriteID[10];
    EntityID digitID[3] = {NullEntity, NullEntity, NullEntity};

private:
};

}  // namespace ECSEngine
