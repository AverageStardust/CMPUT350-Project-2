#pragma once

#include <string>

#include "SystemManager.h"
#include "EntityManager.h"
#include "LocationComponent.h"

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
        for (Entity entity : em) {
            if (entity.template HasComponent<LocationComponent>() &&
                entity.template HasComponent<MovementComponent>()) {
                
                LocationComponent& locationComp = em.template GetComponent<LocationComponent>(entity);
                MovementComponent& movementComp = em.template GetComponent<MovementComponent>(entity);

                locationComp.loc += movementComp.velocity;
            }
        }

        return kContinue;
    }
};

}  // namespace ECSEngine
