#pragma once

#include <string>

#include "EntityManager.h"
#include "InputManager.h"
#include "SoundManager.h"
#include "InputComponent.h"
#include "LocationComponent.h"
#include "CollisionComponent.h"
#include "Keyboard.h"
#include "SystemManager.h"

namespace ECSEngine {

const float kMaxRunVelocity = 15;
const float kDashVelocity = 50;
const float kJumpVelocity = 25;
const float kGroundRunAcceleration = 5;
const float kAirRunAcceleration = 4;
const float kDashInterval = 25;

template <typename... Components>
class InputSystem : public System<Components...> {
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
                entity.template HasComponent<CollisionComponent>()) {

                InputComponent& inputComp = inputPair.second;
                MovementComponent& movementComp = em.template GetComponent<MovementComponent>(entity);
                CollisionComponent& collisionComp = em.template GetComponent<CollisionComponent>(entity);

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
