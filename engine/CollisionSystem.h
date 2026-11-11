#pragma once

#include <string>

#include "SystemManager.h"
#include "EntityManager.h"
#include "CollisionComponent.h"

namespace ECSEngine {

template <typename... Components>
class CollisionSystem : public System<Components...> {
public:
    const std::string& GetName() const override {
        static std::string name("CollisionSystem");
        return name;
    }

    tSystemReturn Run(EngineView<Components...>* engine) override {
        EntityManager<Components...>& em = engine->GetEntityManager();

        for (auto outerIt = em.begin(); outerIt != em.end(); ++outerIt) {
            Entity outerEntity = *outerIt;

            if (outerEntity.template HasComponent<CollisionComponent>()) {
                CollisionComponent& outerCollisionComp = em.template GetComponent<CollisionComponent>(outerEntity);

                for (auto innerIt = std::next(outerIt); innerIt != em.end(); ++innerIt) {
                    Entity innerEntity = *innerIt;

                    if (innerEntity.template HasComponent<CollisionComponent>()) {
                        CollisionComponent& innerCollisionComp = em.template GetComponent<CollisionComponent>(innerEntity);

                        if (outerCollisionComp.worldBoundingBox.IsOverlapping(innerCollisionComp.worldBoundingBox)) {
                            // TODO: resoolve collisions

                            // Handle obstacles
                            if ((outerCollisionComp.doesDamage || innerCollisionComp.doesDamage) /*&& one is player*/) {

                            }

                            // Handle collectibles?

                            // Handle ground
                            // Handle walls
                            // Handle ceiling
                        }
                    }
                }
            }
        }

        return kContinue;
    }
};

}  // namespace ECSEngine
