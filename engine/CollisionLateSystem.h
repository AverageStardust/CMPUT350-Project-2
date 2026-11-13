#pragma once

#include <string>

#include "SystemManager.h"
#include "EntityManager.h"
#include "CollisionComponent.h"
#include "LocationComponent.h"

namespace ECSEngine {

// This system copies the location of the entity and
// saves it into the CollisionComponent.
// For collision processing, see CollisionSystem.h
template <typename... Components>
class CollisionLateSystem : public System<Components...> {
public:
    const std::string& GetName() const override {
        static std::string name("CollisionLateSystem");
        return name;
    }

    tSystemReturn Run(EngineView<Components...>* engine) override {
        EntityManager<Components...>& em = engine->GetEntityManager();
        auto& collisionStore = em.template GetComponentStorage<CollisionComponent>();

        for (auto& collisionPair : collisionStore) {
            Entity<Components...>& entity = em.GetEntity(collisionPair.first);

            if (entity.template HasComponent<LocationComponent>()) {
                CollisionComponent& collisionComp = collisionPair.second;
                LocationComponent& locationComp = em.template GetComponent<LocationComponent>(entity);

                collisionComp.worldBounds = collisionComp.localBounds + locationComp.loc;


                if (entity.template HasComponent<MovementComponent>()) {
                    MovementComponent& movementComp = em.template GetComponent<MovementComponent>(entity);
                    collisionComp.lastVelocity = movementComp.velocity;
                }
            }
        }

        return kContinue;
    }
};

}  // namespace ECSEngine
