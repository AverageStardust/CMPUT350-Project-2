#pragma once

#include <string>

#include "CollisionComponent.h"
#include "EntityManager.h"
#include "LocationComponent.h"
#include "MathUtil.h"
#include "SystemManager.h"

namespace ECSEngine {

template <typename... Components>
class CollisionSystem : public System<Components...> {
public:
    const std::string& GetName() const override {
        static std::string name("CollisionSystem");
        return name;
    }

    void applyDamage(EntityManager<Components...>& em, Entity<Components...>& entity) {
        if (entity.template HasComponent<LocationComponent>() &&
            entity.template HasComponent<RespawnComponent>()) {
            // respawn
            em.template GetComponent<LocationComponent>(entity).loc =
                em.template GetComponent<RespawnComponent>(entity).loc;
        }
    }

    void resolveCollision(EntityManager<Components...>& em, CollisionComponent& otherComp,
                          CollisionComponent& entityComp, Entity<Components...>& entity) {
        LocationComponent locationComp;
        if (entity.template HasComponent<LocationComponent>() &&
            entity.template HasComponent<MovementComponent>()) {
            Rect overlap = otherComp.worldBounds & entityComp.worldBounds;

            if (overlap.Area() > 1) {
                LocationComponent& location = em.template GetComponent<LocationComponent>(entity);
                MovementComponent& movement = em.template GetComponent<MovementComponent>(entity);
                if (overlap.Width() < overlap.Height()) {
                    if (entityComp.worldBounds.Center().x > otherComp.worldBounds.Center().x) {
                        location.loc.x += overlap.Width();
                        movement.velocity.x = std::max(movement.velocity.x, 0.0f);
                        entityComp.onLeftWall = true;
                    } else {
                        location.loc.x -= overlap.Width();
                        movement.velocity.x = std::min(movement.velocity.x, 0.0f);
                        entityComp.onRightWall = true;
                    }
                } else {
                    if (entityComp.worldBounds.Center().y > otherComp.worldBounds.Center().y) {
                        location.loc.y += overlap.Height();
                        movement.velocity.y = std::max(movement.velocity.y, 0.0f);
                        entityComp.onCeiling = true;
                    } else {
                        location.loc.y -= overlap.Height();
                        movement.velocity.y = std::min(movement.velocity.y, 0.0f);
                        entityComp.onGround = true;
                    }
                }
            }
        }
    }

    tSystemReturn Run(EngineView<Components...>* engine) override {
        EntityManager<Components...>& em = engine->GetEntityManager();

        auto& collisionStore = em.template GetComponentStorage<CollisionComponent>();

        for (auto& dynamicPair : collisionStore) {
            CollisionComponent& dynamicComp = dynamicPair.second;

            if (!dynamicComp.isStatic) {
                // reset flags
                dynamicComp.onGround = false;
                dynamicComp.onCeiling = false;
                dynamicComp.onLeftWall = false;
                dynamicComp.onRightWall = false;

                Entity<Components...>& dynamicEntity = em.GetEntity(dynamicPair.first);

                for (auto& otherPair : collisionStore) {
                    if (dynamicPair.first == otherPair.first) continue;
                    // Entity<Components...>& otherEntity = em.GetEntity(otherPair.first);
                    CollisionComponent& otherComp = otherPair.second;

                    if (dynamicComp.worldBounds.IsOverlapping(otherComp.worldBounds)) {
                        bool largerInDynamicDynamicCollision =
                            !otherComp.isStatic &&
                            dynamicComp.localBounds.Area() > otherComp.localBounds.Area();

                        if (!largerInDynamicDynamicCollision)
                            resolveCollision(em, otherComp, dynamicComp, dynamicEntity);

                        if (otherComp.doesDamage) applyDamage(em, dynamicEntity);

                        // Handle collectibles?
                    }
                }
            }
        }

        return kContinue;
    }
};

}  // namespace ECSEngine
