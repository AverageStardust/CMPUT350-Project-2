#pragma once

#include <limits.h>

#include <limits>

#include "SpriteManager.h"
#include "SoundManager.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Keyboard.hpp>

namespace ECSEngine {

struct InputComponent {
    InputComponent() {}
    InputComponent(SpriteID idleSprite, SpriteID walk1Sprite, SpriteID walk2Sprite,
                   SpriteID jumpSprite, SpriteID fallSprite, SpriteID dashSprite,
                   SoundID jumpSound, SoundID landSound, SoundID collectSound, SoundID wallSound,
                   uint maxDashes)
        : idleSprite(idleSprite),
          walk1Sprite(walk1Sprite),
          walk2Sprite(walk2Sprite),
          jumpSprite(jumpSprite),
          fallSprite(fallSprite),
          dashSprite(dashSprite),

          jumpSound(jumpSound),
          landSound(landSound),
          collectSound(collectSound),
          pushAgainstWallWhileFallingSound(wallSound),

          maxDashes(maxDashes) {}

    SpriteID idleSprite;
    SpriteID walk1Sprite;
    SpriteID walk2Sprite;
    SpriteID jumpSprite;
    SpriteID fallSprite;
    SpriteID dashSprite;
    SoundID jumpSound;
    SoundID landSound;
    SoundID collectSound;
    SoundID pushAgainstWallWhileFallingSound;
    //soundID dashSound;
    //soundID footstepSound;
    float walkCycle = 0;
    uint maxDashes = 1;
    uint dashesLeft = 0;
    uint dashLockout = 0;
    uint jumpTime = 0;
    uint jumpBufferTime = 0;
    uint timeSinceGround = std::numeric_limits<uint>::max() / 2;
    uint timeSinceLeftWall = std::numeric_limits<uint>::max() / 2;
    uint timeSinceRightWall = std::numeric_limits<uint>::max() / 2;
    uint wallSoundActive = false;
    uint landSoundActive = false;
};

}  // namespace ECSEngine
