#pragma once

#include <string>

#include "SystemManager.h"
#include "EntityManager.h"
#include "CollisionComponent.h"
#include "LocationComponent.h"
#include "SpriteComponent.h"

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

        for (Entity entity : em) {
            if (entity.template HasComponent<CollisionComponent>() &&
                entity.template HasComponent<LocationComponent>()) {

                LocationComponent& locationComp = em.template GetComponent<LocationComponent>(entity);
                CollisionComponent& collisionComp = em.template GetComponent<CollisionComponent>(entity);

                collisionComp.worldBoundingBox = collisionComp.localBoundingBox + locationComp.loc;
            }
        }

        return kContinue;
    }
};

}  // namespace ECSEngine
