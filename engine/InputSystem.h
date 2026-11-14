#pragma once

#include <string>

#include "CameraSystem.h"
#include "CollisionComponent.h"
#include "EntityManager.h"
#include "InputComponent.h"
#include "InputManager.h"
#include "Keyboard.h"
#include "LocationComponent.h"
#include "SoundManager.h"
#include "SpriteComponent.h"
#include "SystemManager.h"

namespace ECSEngine {

const float kMaxRunVelocity = 320;
const float kGroundRunAcceleration = 85;
const float kAirRunAcceleration = 10;

const int kCoyoteTime = 9;
const int kJumpBufferTime = 9;
const int kVerticalJumpTime = 17;
const int kWallJumpTime = 12;
const float kJumpInitalAcceleration = 350;
const float kJumpExtendedAcceleration = 60;
const float kJumpWallVerticalAcceleration = 250;
const float kJumpWallHorizontalAcceleration = 220;

const float kDashVelocity = 500;
const float kDashInterval = 25;
const float kDashDelay = 30;

const float kGroundFriction = 0.15;
const float kAirDrag = 0.035;
const float kWallSlideFriction = 0.15;

const int kLeftKey = Key::Left;
const int kRightKey = Key::Right;
const int kDownKey = Key::Down;
const int kUpKey = Key::Up;
const int kJumpKey = Key::Z;
const int kDashKey = Key::C;

template <typename... Components>
class InputSystem : public System<Components...> {
private:
    void handleRunning(InputManager& im, MovementComponent& movementComp,
                       CollisionComponent& collisionComp) {
        // ground and air running
        if (collisionComp.onGround) {
            if (im.KeyIsDown(kRightKey) && movementComp.velocity.x < kMaxRunVelocity) {
                movementComp.velocity.x += kGroundRunAcceleration;
            }
            if (im.KeyIsDown(kLeftKey) && movementComp.velocity.x > -kMaxRunVelocity) {
                movementComp.velocity.x -= kGroundRunAcceleration;
            }
        } else {
            if (im.KeyIsDown(kRightKey) && movementComp.velocity.x < kMaxRunVelocity) {
                movementComp.velocity.x += kAirRunAcceleration;
            }
            if (im.KeyIsDown(kLeftKey) && movementComp.velocity.x > -kMaxRunVelocity) {
                movementComp.velocity.x -= kAirRunAcceleration;
            }
            if (im.KeyIsDown(kDownKey)) {
                movementComp.velocity.y += kAirRunAcceleration;
            }
        }
    }

    void handleCoyoteAndBuffering(InputManager& im, InputComponent& inputComp,
                                  CollisionComponent& collisionComp) {
        // jump buffering
        if (im.KeyJustPressed(kJumpKey)) {
            inputComp.jumpBufferTime = kJumpBufferTime;
        } else if (inputComp.jumpBufferTime > 0) {
            inputComp.jumpBufferTime--;
        }

        if (im.KeyJustPressed(kDashKey)) {
            inputComp.dashBufferTime = kJumpBufferTime;
        } else if (inputComp.dashBufferTime > 0) {
            inputComp.dashBufferTime--;
        }

        // dash delay
        if (inputComp.dashLockout > 0) {
            inputComp.dashLockout--;
        }

        // coyote time
        if (collisionComp.onGround) {
            inputComp.timeSinceGround = 0;
            inputComp.dashesLeft = 1;  // restore dashes
        } else {
            inputComp.timeSinceGround++;
        }
        if (collisionComp.onLeftWall) {
            inputComp.timeSinceLeftWall = 0;
            inputComp.dashesLeft = 1;  // restore dashes
        } else {
            inputComp.timeSinceLeftWall++;
        }
        if (collisionComp.onRightWall) {
            inputComp.timeSinceRightWall = 0;
            inputComp.dashesLeft = 1;  // restore dashes
        } else {
            inputComp.timeSinceRightWall++;
        }
    }

    void lockJump(InputComponent& inputComp) {
        inputComp.timeSinceGround = kCoyoteTime;
        inputComp.jumpBufferTime = 0;
    }

    void consumeDash(InputComponent& inputComp) {
        inputComp.dashesLeft--;
        inputComp.dashLockout = kDashDelay;
        inputComp.dashBufferTime = 0;
    }

    void HandleDashing(EngineView<Components...>* engine, SoundManager& sm, InputManager& im,
                       InputComponent& inputComp, MovementComponent& movementComp,
                       GravityComponent& gravityComp) {
        if (inputComp.dashBufferTime > 0) {
            inputComp.dashBufferTime--;
            if (inputComp.dashesLeft > 0 && inputComp.dashLockout == 0) {
                if (im.KeyIsDown(kRightKey) && !im.KeyIsDown(kLeftKey)) {
                    movementComp.velocity.x = kDashVelocity;
                    movementComp.velocity.y = 0;
                    gravityComp.disabledTime = kDashInterval;
                    consumeDash(inputComp);
                    CameraSystem<Components...>::ApplyScreenShake(engine, {kDashVelocity, 0});
                    sm.Play(inputComp.dashSound, 0.7);
                } else if (im.KeyIsDown(kLeftKey) && !im.KeyIsDown(kRightKey)) {
                    movementComp.velocity.x = -kDashVelocity;
                    movementComp.velocity.y = 0;
                    gravityComp.disabledTime = kDashInterval;
                    consumeDash(inputComp);
                    CameraSystem<Components...>::ApplyScreenShake(engine, {-kDashVelocity, 0});
                    sm.Play(inputComp.dashSound, 0.7);
                }
            }
        }
    }

    void handleJumping(SoundManager& sm, InputManager& im, InputComponent& inputComp,
                       MovementComponent& movementComp) {
        if (inputComp.jumpBufferTime > 0) {
            // veritcal jump
            if (inputComp.timeSinceGround < kCoyoteTime) {
                lockJump(inputComp);
                inputComp.jumpTime = kVerticalJumpTime;
                movementComp.velocity.y = -kJumpInitalAcceleration;
                sm.Play(inputComp.jumpSound, 0.4);
            } else if (inputComp.timeSinceLeftWall < kCoyoteTime &&
                       inputComp.timeSinceLeftWall < inputComp.timeSinceRightWall) {
                // wall jump left
                lockJump(inputComp);
                inputComp.jumpTime = kWallJumpTime;
                movementComp.velocity.x = kJumpWallHorizontalAcceleration;
                movementComp.velocity.y = -kJumpWallVerticalAcceleration;
                sm.Play(inputComp.jumpSound, 0.3);
            } else if (inputComp.timeSinceRightWall < kCoyoteTime) {
                // wall jump right
                lockJump(inputComp);
                inputComp.jumpTime = kWallJumpTime;
                movementComp.velocity.x = -kJumpWallHorizontalAcceleration;
                movementComp.velocity.y = -kJumpWallVerticalAcceleration;
                sm.Play(inputComp.jumpSound, 0.3);
            }
        } else if (inputComp.jumpTime > 0) {
            // extended jump
            if (im.KeyIsDown(kJumpKey)) {
                movementComp.velocity.y -= kJumpExtendedAcceleration;
                if (inputComp.jumpTime == 2) {
                    sm.Play(inputComp.jumpHighSound, 0.6);
                }
            }
            inputComp.jumpTime--;
        }
    }

    void handleWallSliding(SoundManager& sm, InputManager& im, InputComponent& inputComp,
                           MovementComponent& movementComp, CollisionComponent& collisionComp) {
        // wall slide
        if (((im.KeyIsDown(kRightKey) && collisionComp.onRightWall) ||
             (im.KeyIsDown(kLeftKey) && collisionComp.onLeftWall)) &&
            movementComp.velocity.y > 0) {
            movementComp.velocity.y *= 1.0f - kWallSlideFriction;
            if (inputComp.slideSoundTimeout <= 0) {
                sm.Play(inputComp.landingSound, 0.03);
                inputComp.slideSoundTimeout = 5;
            }
            inputComp.slideSoundTimeout--;
        } else {
            inputComp.slideSoundTimeout = 0;
        }
    }

    void handleFriction(MovementComponent& movementComp, CollisionComponent& collisionComp) {
        // drag / friction
        if (collisionComp.onGround) {
            movementComp.velocity.x *= 1.0f - kGroundFriction;
        } else {
            movementComp.velocity.x *= 1.0f - kAirDrag;
            if (movementComp.velocity.y < 0) { // only vertical drag going up
                movementComp.velocity.y *= 1.0f - kAirDrag * 2;
            }
        }
    }

    void handleAnimation(SoundManager& sm, InputComponent& inputComp,
                         MovementComponent& movementComp, CollisionComponent& collisionComp,
                         SpriteComponent& spriteComp) {
        if (std::abs(movementComp.velocity.x) > 30) {
            spriteComp.flip = movementComp.velocity.x < 0;
        }

        if (collisionComp.onGround) {
            if (std::abs(movementComp.velocity.x) < 30) {
                spriteComp.spriteID = inputComp.idleSprite;
                inputComp.walkCycle = 0.6;
            } else {
                inputComp.walkCycle += 0.01 + std::abs(movementComp.velocity.x) / 6000;
                if (inputComp.walkCycle > 1) {
                    inputComp.walkCycle -= 1;
                    sm.Play(inputComp.footstepSound, 0.12);
                }

                if (inputComp.walkCycle < 0.5) {
                    spriteComp.spriteID = inputComp.walk2Sprite;
                } else {
                    spriteComp.spriteID = inputComp.walk1Sprite;
                }
            }
        } else {
            inputComp.walkCycle = 0.6;
            if (inputComp.dashLockout > 0) {
                spriteComp.spriteID = inputComp.dashSprite;
            } else {
                if (movementComp.velocity.y < 0) {
                    spriteComp.spriteID = inputComp.jumpSprite;
                } else {
                    spriteComp.spriteID = inputComp.fallSprite;
                }
            }
        }
    }

public:
    const std::string& GetName() const override {
        static std::string name("InputSystem");
        return name;
    }

    tSystemReturn Run(EngineView<Components...>* engine) override {
        InputManager& im = engine->GetInputManager();
        SoundManager& sm = engine->GetSoundManager();
        EntityManager<Components...>& em = engine->GetEntityManager();
        auto& inputStore = em.template GetComponentStorage<InputComponent>();

        for (auto& inputPair : inputStore) {
            Entity<Components...>& entity = em.GetEntity(inputPair.first);

            if (entity.template HasComponent<MovementComponent>() &&
                entity.template HasComponent<CollisionComponent>() &&
                entity.template HasComponent<GravityComponent>() &&
                entity.template HasComponent<SpriteComponent>()) {
                InputComponent& inputComp = inputPair.second;
                MovementComponent& movementComp =
                    em.template GetComponent<MovementComponent>(entity);
                CollisionComponent& collisionComp =
                    em.template GetComponent<CollisionComponent>(entity);
                GravityComponent& gravityComp = em.template GetComponent<GravityComponent>(entity);
                SpriteComponent& spriteComp = em.template GetComponent<SpriteComponent>(entity);

                handleRunning(im, movementComp, collisionComp);
                handleCoyoteAndBuffering(im, inputComp, collisionComp);
                handleJumping(sm, im, inputComp, movementComp);
                HandleDashing(engine, sm, im, inputComp, movementComp, gravityComp);
                handleWallSliding(sm, im, inputComp, movementComp, collisionComp);
                handleFriction(movementComp, collisionComp);
                handleAnimation(sm, inputComp, movementComp, collisionComp, spriteComp);
            }
        }

        return kContinue;
    }
};

}  // namespace ECSEngine
