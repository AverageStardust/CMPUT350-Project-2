#pragma once

#include "MathUtil.h"
#include "SpriteManager.h"

namespace ECSEngine {

// The sprite rectangle is relative to the entity location in the world
// This is only defined if an entity also has a LocationComponent.
class SpawnComponent {
public:
    SpriteID spawnSpriteID;
    Point2D spawnSize;
    int timeToNextSpawn;
    int timeBetweenSpawns;  // in frames
    int totalSpawns;

private:
};

}  // namespace ECSEngine
