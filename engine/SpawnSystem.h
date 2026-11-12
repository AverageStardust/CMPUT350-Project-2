#pragma once

#include <string>

#include "SpriteComponent.h"
#include "SystemManager.h"
#include "EntityManager.h"
#include "LocationComponent.h"
#include "SpawnComponent.h"

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
                LocationComponent& locationComp = em.template GetComponent<LocationComponent>(entity);

                if (spawnComp.totalSpawns > 0) {
                    if (spawnComp.timeToNextSpawn > 0) {
                        spawnComp.timeToNextSpawn -= 1;
                    }
                    else {
                        spawnComp.timeToNextSpawn = spawnComp.timeBetweenSpawns;
                        spawnComp.totalSpawns -= 1;

                        EntityID spawnedID = em.CreateEntity();

                        // Add basic components
                        em.AddComponent(spawnedID, LocationComponent{ .loc = locationComp.loc });

                        em.AddComponent(spawnedID, SpriteComponent{ .spriteID = spawnComp.spawnTextureID,
                                                                    .offset = Point2D(0, 0),
                                                                    .worldSpace = true });

                        // TODO: specialize this code for star entities

                    }
                }
            }
        }

        return kContinue;
    }
};

}  // namespace ECSEngine
