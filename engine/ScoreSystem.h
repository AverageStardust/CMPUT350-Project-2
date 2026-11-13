#pragma once

#include <cmath>
#include <string>

#include "Entity.h"
#include "EntityManager.h"
#include "MathUtil.h"
#include "ScoreComponent.h"
#include "SpriteComponent.h"
#include "SystemManager.h"

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

        Storage<std::pair<EntityID, ScoreComponent>>& scoreStore =
            em.template GetComponentStorage<ScoreComponent>();

        for (std::pair<EntityID, ScoreComponent>& pair : scoreStore) {
            ScoreComponent& scoreComp = pair.second;

            int digitCount = sizeof(scoreComp.digitID) / sizeof(scoreComp.digitID[0]);
            for (int i = 0; i < digitCount; i++) {
                if (scoreComp.digitID[i] == NullEntity) {
                    scoreComp.digitID[i] = em.CreateEntity();
                }

                EntityID& digitID = scoreComp.digitID[i];

                int placeValue = int(std::pow(10, digitCount - i - 1));
                int digitValue = (scoreComp.score / placeValue) % 10;

                if (!em.template HasComponent<SpriteComponent>(digitID)) {
                    em.AddComponent(digitID, SpriteComponent{
                                                 .spriteID = scoreComp.digitSpriteID[0],
                                                 .worldSpace = false,
                                             });
                }

                SpriteComponent& spriteComp = em.template GetComponent<SpriteComponent>(digitID);
                spriteComp.offset = scoreComp.offset + Point2D{i * scoreComp.spacing, 0};
                spriteComp.spriteID = scoreComp.digitSpriteID[digitValue];
            }
        }

        return kContinue;
    }
};

}  // namespace ECSEngine
