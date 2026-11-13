#pragma once

#include "Entity.h"
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

        Storage<std::pair<EntityID, SpriteComponent>>& spriteStore =
            em.template GetComponentStorage<SpriteComponent>();

        for (std::pair<EntityID, SpriteComponent> pair : spriteStore) {
            EntityID entityID = pair.first;
            SpriteComponent& spriteComp = pair.second;

            Point2D location = {};
            if (em.template HasComponent<LocationComponent>(entityID)) {
                location = em.template GetComponent<LocationComponent>(entityID).loc;
            }

            sf::Sprite sprite = sm.GetSprite(spriteComp.spriteID);
            if (spriteComp.worldSpace) {
                wm.DrawSpriteWorld(sprite, location + spriteComp.offset, spriteComp.flip);
            } else {
                wm.DrawSpriteWindow(sprite, location + spriteComp.offset, 1, spriteComp.flip);
            }
        }

        return kContinue;
    }
};

}  // namespace ECSEngine
