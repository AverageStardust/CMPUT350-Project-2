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

const float kMaxRunVelocity = 300;
const float kDashVelocity = 50;
const int kJumpTime = 10;
const float kGroundRunAcceleration = 70;
const float kAirRunAcceleration = 10;
const float kJumpAcceleration = 150;
const float kDashInterval = 25;
const float kGroundFriction = 0.15;
const float kAirDrag = 0.04;

template <typename... Components>
class InputSystem : public System<Components...> {
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

                if (collisionComp.onGround) {
                    if (im.KeyIsDown(Key::D) && movementComp.velocity.x < kMaxRunVelocity) {
                        movementComp.velocity.x += kGroundRunAcceleration;
                    }
                    if (im.KeyIsDown(Key::A) && movementComp.velocity.x > -kMaxRunVelocity) {
                        movementComp.velocity.x -= kGroundRunAcceleration;
                    }
                    if (!im.KeyIsDown(Key::D) && !im.KeyIsDown(Key::A)) {
                        movementComp.velocity.x *= 1.0f - kGroundFriction;
                    }
                    inputComp.airTime = 0;
                } else {
                    if (im.KeyIsDown(Key::D) && movementComp.velocity.x < kMaxRunVelocity) {
                        movementComp.velocity.x += kAirRunAcceleration;
                    }
                    if (im.KeyIsDown(Key::A) && movementComp.velocity.x > -kMaxRunVelocity) {
                        movementComp.velocity.x -= kAirRunAcceleration;
                    }
                    movementComp.velocity.x *= 1.0f - kAirDrag;
                    inputComp.airTime++;
                }

                if (inputComp.airTime < kJumpTime) {
                    if (im.KeyIsDown(Key::Space)) {
                        if (inputComp.airTime == 0) {
                            movementComp.velocity.y -= kJumpAcceleration;
                        } else {
                            movementComp.velocity.y -= kJumpAcceleration / 4;
                        }
                    }
                }

                // Deal with dashing before standard x movement (normal movement limits velocity)

                // Moving left/right
                // moving against wall - play a sound, tell game you're moving into wall!
                // increase velocity up to a maximum

                // Holding down
                movementComp.velocity += Point2D();

                // Jumping
            }
        }

        return kContinue;
    }
};

}  // namespace ECSEngine
