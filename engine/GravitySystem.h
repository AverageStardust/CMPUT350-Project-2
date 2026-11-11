#pragma once

#include <string>

#include "SystemManager.h"
#include "LocationComponent.h"
#include "CollisionComponent.h"

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

        for (Entity entity : em) {
            if (entity.template HasComponent<GravityComponent>() &&
                entity.template HasComponent<MovementComponent>() &&
                entity.template HasComponent<CollisionComponent>()) {
                
                GravityComponent& gravityComp = em.template GetComponent<GravityComponent>(entity);
                MovementComponent& movementComp = em.template GetComponent<MovementComponent>(entity);
                CollisionComponent& collisionComp = em.template GetComponent<CollisionComponent>(entity);

                if (!collisionComp.onGround) {
                    movementComp.velocity += gravityComp.acceleration;
                }
            }
        }

        return kContinue;
    }
};

}  // namespace ECSEngine
