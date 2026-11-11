#pragma once

#include "EntityManager.h"
#include "LocationComponent.h"
#include "MathUtil.h"
#include "SpriteComponent.h"
#include "SpriteManager.h"
#include "SystemManager.h"
#include "WindowManager.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace ECSEngine {

template <typename... Components>
class SpriteSystem : public System<Components...> {
public:
    const std::string& GetName() const override {
        static std::string name("SpriteSystem");
        return name;
    }

    tSystemReturn Run(EngineView<Components...>* engine) override {
        WindowManager& wm = engine->GetWindowManager();
        SpriteManager& sm = engine->GetSpriteManager();

        EntityManager<Components...>& em = engine->GetEntityManager();

        for (Entity entity : em) {
            if (entity.template HasComponent<LocationComponent>() &&
                entity.template HasComponent<SpriteComponent>()) {
                LocationComponent& locationComp = em.template GetComponent<LocationComponent>(entity);
                SpriteComponent& spriteComp = em.template GetComponent<SpriteComponent>(entity);

                wm.DrawSpriteWorld(sm.GetSprite(spriteComp.spriteID), spriteComp.bounds + locationComp.loc);
            }
        }

        return kContinue;
    }
};

}  // namespace ECSEngine
