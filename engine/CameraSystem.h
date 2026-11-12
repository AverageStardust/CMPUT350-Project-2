#pragma once

#include "CameraComponent.h"
#include "Entity.h"
#include "EntityManager.h"
#include "LocationComponent.h"
#include "SystemManager.h"
#include "WindowManager.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace ECSEngine {

template <typename... Components>
class CameraSystem : public System<Components...> {
public:
    const std::string& GetName() const override {
        static std::string name = "CameraSystem";
        return name;
    }

    tSystemReturn Run(EngineView<Components...>* engine) override {
        EntityManager<Components...>& em = engine->GetEntityManager();
        WindowManager& wm = engine->GetWindowManager();

        auto& cameraStore = em.template GetComponentStorage<CameraComponent>();

        for (auto& cameraPair : cameraStore) {
            Entity<Components...>& entity = em.GetEntity(cameraPair.first);
            CameraComponent& camera = cameraPair.second;
            if (camera.active == false) continue;

            if (entity.template HasComponent<CameraFollower>()) {
                CameraFollower& follower = em.template GetComponent<CameraFollower>(entity);
                if (em.template HasComponent<LocationComponent>(follower.targetID)) {
                    LocationComponent& targetLocation =
                        em.template GetComponent<LocationComponent>(follower.targetID);
                    Point2D windowPosition = wm.WorldToWindow(targetLocation.loc);

                    if (!follower.hardExclusion.IsInside(windowPosition)) {
                        Point2D screenDistance = windowPosition - follower.hardExclusion.NearestInside(windowPosition);
                        camera.position += wm.WindowDistanceToWorld(screenDistance);
                    } else if (!follower.softExclusion.IsInside(windowPosition)) {
                        camera.position = camera.position.MoveTowards(targetLocation.loc, follower.speed / FRAME_RATE);
                    }
                }
            }

            wm.SetWorldCenter({camera.position.x, 384});
            wm.SetWorldScale(camera.worldUnitsPerPixel);

            break;  // found camera, done
        }

        return kContinue;
    }
};

}  // namespace ECSEngine
