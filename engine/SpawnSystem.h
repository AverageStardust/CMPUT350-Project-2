#pragma once

#include <string>

#include "CollisionComponent.h"
#include "EntityManager.h"
#include "LocationComponent.h"
#include "MathUtil.h"
#include "SpawnComponent.h"
#include "SpriteComponent.h"
#include "SystemManager.h"

namespace ECSEngine {

template <typename... Components>
class SpawnSystem : public System<Components...> {
public:
    const std::string& GetName() const override {
        static std::string name("SpawnSystem");
        return name;
    }

    tSystemReturn Run(EngineView<Components...>* engine) override {
        EntityManager<Components...>& em = engine->GetEntityManager();

        for (Entity entity : em) {
            if (entity.template HasComponent<SpawnComponent>() &&
                entity.template HasComponent<LocationComponent>()) {
                SpawnComponent& spawnComp = em.template GetComponent<SpawnComponent>(entity);
                LocationComponent& locationComp =
                    em.template GetComponent<LocationComponent>(entity);

                if (spawnComp.totalSpawns > 0) {
                    if (spawnComp.timeToNextSpawn > 0) {
                        spawnComp.timeToNextSpawn -= 1;
                    } else {
                        spawnComp.timeToNextSpawn = spawnComp.timeBetweenSpawns;
                        spawnComp.totalSpawns -= 1;

                        EntityID spawnedID = em.CreateEntity();

                        // Add basic components
                        Point2D spawnLocation =
                            locationComp.loc + Point2D{32, -8 - spawnComp.spawnSize.y / 2};
                        em.AddComponent(spawnedID, LocationComponent{.loc = spawnLocation});

                        em.AddComponent(spawnedID,
                                        SpriteComponent{.spriteID = spawnComp.spawnSpriteID,
                                                        .offset = spawnComp.spawnSize / -2,
                                                        .worldSpace = true});

                        em.AddComponent(spawnedID, CollisionComponent{
                                                       .isCollectable = true,
                                                       .localBounds = {spawnComp.spawnSize / -2,
                                                                       spawnComp.spawnSize / 2}});

                        em.AddComponent(spawnedID,
                                        MovementComponent{.velocity = Point2D((rand() % 512) - 256,
                                                                              -(rand() % 128))});

                        em.AddComponent(spawnedID, GravityComponent(Point2D(0, 100), 0));
                    }
                }
            }
        }

        return kContinue;
    }
};

}  // namespace ECSEngine
