#pragma once

#include <string>

#include "SystemManager.h"
#include "EntityManager.h"
#include "ScoreComponent.h"

namespace ECSEngine {

template <typename... Components>
class ScoreSystem : public System<Components...> {
public:
    const std::string& GetName() const override {
        static std::string name("ScoreSystem");
        return name;
    }

    tSystemReturn Run(EngineView<Components...>* engine) override {
        EntityManager<Components...>& em = engine->GetEntityManager();

        for (Entity entity : em) {
            if (entity.template HasComponent<ScoreComponent>()) {

                ScoreComponent& scoreComp = em.template GetComponent<ScoreComponent>(entity);

                int divAmount = 1;
                for (EntityID eID : scoreComp.digitID) {
                    int digitValue = (scoreComp.score / divAmount) % 10;
                    Entity curEntity = em.GetEntity(eID);

                    if (curEntity.template HasComponent<SpriteComponent>()) {
                        SpriteComponent& curSpriteComp = em.template GetComponent<SpriteComponent>(curEntity);
                        curSpriteComp.spriteID = scoreComp.digitTextureID[digitValue];
                    }

                    divAmount *= 10;
                }
            }
        }

        return kContinue;
    }
};

}  // namespace ECSEngine
