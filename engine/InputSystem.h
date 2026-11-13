#pragma once

#include <string>

#include "CollisionComponent.h"
#include "EntityManager.h"
#include "InputComponent.h"
#include "InputManager.h"
#include "Keyboard.h"
#include "LocationComponent.h"
#include "SoundManager.h"
#include "SystemManager.h"

namespace ECSEngine {

const float kMaxRunVelocity = 320;
const float kGroundRunAcceleration = 85;
const float kAirRunAcceleration = 10;

const int kCoyoteTime = 9;
const int kJumpBufferTime = 9;
const int kVerticalJumpTime = 10;
const int kWallJumpTime = 5;
const float kJumpInitalAcceleration = 400;
const float kJumpExtendedAcceleration = 55;
const float kJumpWallVerticalAcceleration = 220;
const float kJumpWallHorizontalAcceleration = 200;

const float kDashVelocity = 400;
const float kDashInterval = 25;

const float kGroundFriction = 0.15;
const float kAirDrag = 0.035;
const float kWallSlideFriction = 0.15;

template <typename... Components>
class InputSystem : public System<Components...> {
private:
    void HandleRunning(InputManager& im, MovementComponent& movementComp,
                       CollisionComponent& collisionComp) {
        // ground and air running
        if (collisionComp.onGround) {
            if (im.KeyIsDown(Key::D) && movementComp.velocity.x < kMaxRunVelocity) {
                movementComp.velocity.x += kGroundRunAcceleration;
            }
            if (im.KeyIsDown(Key::A) && movementComp.velocity.x > -kMaxRunVelocity) {
                movementComp.velocity.x -= kGroundRunAcceleration;
            }
        } else {
            if (im.KeyIsDown(Key::D) && movementComp.velocity.x < kMaxRunVelocity) {
                movementComp.velocity.x += kAirRunAcceleration;
            }
            if (im.KeyIsDown(Key::A) && movementComp.velocity.x > -kMaxRunVelocity) {
                movementComp.velocity.x -= kAirRunAcceleration;
            }
            if (im.KeyIsDown(Key::S)) {
                movementComp.velocity.y += kAirRunAcceleration;
            }
        }
    }

    void HandleJumpCoyoteAndBuffering(InputManager& im, InputComponent& inputComp,
                                      CollisionComponent& collisionComp) {
        // jump buffering
        if (im.KeyJustPressed(Key::Space)) {
            inputComp.jumpBufferTime = kJumpBufferTime;
        } else if (inputComp.jumpBufferTime > 0) {
            inputComp.jumpBufferTime--;
        }

        // coyote time
        if (collisionComp.onGround) {
            inputComp.timeSinceGround = 0;
        } else {
            inputComp.timeSinceGround++;
        }
        if (collisionComp.onLeftWall) {
            inputComp.timeSinceLeftWall = 0;
        } else {
            inputComp.timeSinceLeftWall++;
        }
        if (collisionComp.onRightWall) {
            inputComp.timeSinceRightWall = 0;
        } else {
            inputComp.timeSinceRightWall++;
        }
    }

    void JumpForTime(InputComponent& inputComp, uint time) {
        inputComp.timeSinceGround = kCoyoteTime;
        inputComp.jumpBufferTime = 0;
        inputComp.jumpTime = time;
    }

    void HandleJumping(InputManager& im, InputComponent& inputComp,
                       MovementComponent& movementComp) {
        if (inputComp.jumpBufferTime > 0) {
            // veritcal jump
            if (inputComp.timeSinceGround < kCoyoteTime) {
                JumpForTime(inputComp, kVerticalJumpTime);
                movementComp.velocity.y = -kJumpInitalAcceleration;
            } else if (inputComp.timeSinceLeftWall < inputComp.timeSinceRightWall) {
                // wall jump left
                if (inputComp.timeSinceLeftWall < kCoyoteTime) {
                    JumpForTime(inputComp, kVerticalJumpTime);
                    movementComp.velocity.x = kJumpWallHorizontalAcceleration;
                    movementComp.velocity.y = -kJumpWallVerticalAcceleration;
                }
            } else {
                // wall jump right
                if (inputComp.timeSinceRightWall < kCoyoteTime) {
                    JumpForTime(inputComp, kVerticalJumpTime);
                    movementComp.velocity.x = -kJumpWallHorizontalAcceleration;
                    movementComp.velocity.y = -kJumpWallVerticalAcceleration;
                }
            }
        } else if (inputComp.jumpTime > 0) {
            // extended jump
            inputComp.jumpTime--;
            if (im.KeyIsDown(Key::Space)) {
                movementComp.velocity.y -= kJumpExtendedAcceleration;
            }
        }
    }

    void HandleWallSliding(InputManager& im, MovementComponent& movementComp,
                           CollisionComponent& collisionComp) {
        // wall slide
        if (((im.KeyIsDown(Key::D) && collisionComp.onRightWall) ||
             (im.KeyIsDown(Key::A) && collisionComp.onLeftWall)) &&
            movementComp.velocity.y > 0) {
            movementComp.velocity.y *= 1.0f - kWallSlideFriction;
        }
    }

    void HandleFriction(MovementComponent& movementComp, CollisionComponent& collisionComp) {
        // drag / friction
        if (collisionComp.onGround) {
            movementComp.velocity.x *= 1.0f - kGroundFriction;
        } else {
            movementComp.velocity.x *= 1.0f - kAirDrag;
            if (movementComp.velocity.y < 0 || movementComp.velocity.y > kMaxRunVelocity) {
                movementComp.velocity.y *= 1.0f - kAirDrag;
            }
        }
    }

    // void A(InputManager& im, InputComponent& inputComp, MovementComponent& movementComp,
    //        CollisionComponent& collisionComp) {}

public:
    const std::string& GetName() const override {
        static std::string name("InputSystem");
        return name;
    }

    tSystemReturn Run(EngineView<Components...>* engine) override {
        InputManager& im = engine->GetInputManager();
        // SoundManager& sm = engine->GetSoundManager();
        EntityManager<Components...>& em = engine->GetEntityManager();
        auto& inputStore = em.template GetComponentStorage<InputComponent>();

        for (auto& inputPair : inputStore) {
            Entity<Components...>& entity = em.GetEntity(inputPair.first);

            if (entity.template HasComponent<MovementComponent>() &&
                entity.template HasComponent<CollisionComponent>()) {
                InputComponent& inputComp = inputPair.second;
                MovementComponent& movementComp =
                    em.template GetComponent<MovementComponent>(entity);
                CollisionComponent& collisionComp =
                    em.template GetComponent<CollisionComponent>(entity);

                HandleRunning(im, movementComp, collisionComp);
                HandleJumpCoyoteAndBuffering(im, inputComp, collisionComp);
                HandleJumping(im, inputComp, movementComp);
                HandleWallSliding(im, movementComp, collisionComp);
                HandleFriction(movementComp, collisionComp);
            }
        }

        return kContinue;
    }
};

}  // namespace ECSEngine
