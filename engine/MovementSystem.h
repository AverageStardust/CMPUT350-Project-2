#pragma once

#include <string>

#include "EntityManager.h"
#include "LocationComponent.h"
#include "SystemManager.h"
#include "WindowManager.h"

namespace ECSEngine {

template <typename... Components>
class MovementSystem : public System<Components...> {
public:
    const std::string& GetName() const override {
        static std::string name("MovementSystem");
        return name;
    }

    tSystemReturn Run(EngineView<Components...>* engine) override {
        EntityManager<Components...>& em = engine->GetEntityManager();

        auto& movementStore = em.template GetComponentStorage<MovementComponent>();

        for (auto& movementPair : movementStore) {
            Entity<Components...>& entity = em.GetEntity(movementPair.first);

            if (entity.template HasComponent<LocationComponent>()) {
                MovementComponent& movementComp = movementPair.second;
                LocationComponent& locationComp =
                    em.template GetComponent<LocationComponent>(entity);

                locationComp.loc += movementComp.velocity / FRAMES_PER_SECOND;
            }
        }

        return kContinue;
    }
};

}  // namespace ECSEngine
