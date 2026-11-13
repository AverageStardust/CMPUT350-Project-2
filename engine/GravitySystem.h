#pragma once

#include <string>

#include "CollisionComponent.h"
#include "EntityManager.h"
#include "LocationComponent.h"
#include "SystemManager.h"
#include "WindowManager.h"

namespace ECSEngine {

template <typename... Components>
class GravitySystem : public System<Components...> {
public:
    const std::string& GetName() const override {
        static std::string name("GravitySystem");
        return name;
    }

    tSystemReturn Run(EngineView<Components...>* engine) override {
        EntityManager<Components...>& em = engine->GetEntityManager();
        auto& gravityStore = em.template GetComponentStorage<GravityComponent>();

        for (auto& gravityPair : gravityStore) {
            Entity<Components...>& entity = em.GetEntity(gravityPair.first);

            if (entity.template HasComponent<MovementComponent>() &&
                entity.template HasComponent<CollisionComponent>()) {

                GravityComponent& gravityComp = gravityPair.second;
                MovementComponent& movementComp =
                    em.template GetComponent<MovementComponent>(entity);
                CollisionComponent& collisionComp =
                    em.template GetComponent<CollisionComponent>(entity);

                if (!collisionComp.onGround) {
                    movementComp.velocity += gravityComp.acceleration / FRAMES_PER_SECOND;
                }
            }
        }

        return kContinue;
    }
};

}  // namespace ECSEngine
