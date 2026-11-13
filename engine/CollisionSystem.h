#pragma once

#include <string>

#include "CollisionComponent.h"
#include "EntityManager.h"
#include "LocationComponent.h"
#include "InputComponent.h"
#include "ScoreComponent.h"
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
        bool massRatio = 1;

        if (!otherComp.isStatic) {
            float entityArea = entityComp.localBounds.Area();
            float otherArea = otherComp.localBounds.Area();
            massRatio = otherArea / (entityArea + otherArea);
        }

        LocationComponent locationComp;
        if (entity.template HasComponent<LocationComponent>() &&
            entity.template HasComponent<MovementComponent>()) {
            Rect overlap = otherComp.worldBounds & entityComp.worldBounds;

            if (overlap.Area() > 1) {
                LocationComponent& location = em.template GetComponent<LocationComponent>(entity);
                MovementComponent& movement = em.template GetComponent<MovementComponent>(entity);
                if (overlap.Width() < overlap.Height()) {
                    if (entityComp.worldBounds.Center().x > otherComp.worldBounds.Center().x) {
                        location.loc.x += overlap.Width() * massRatio;
                        movement.velocity.x +=
                            (otherComp.lastVelocity.x - movement.velocity.x) * massRatio;
                        entityComp.onLeftWall = true;
                    } else {
                        location.loc.x -= overlap.Width() * massRatio;
                        movement.velocity.x +=
                            (otherComp.lastVelocity.x - movement.velocity.x) * massRatio;
                        entityComp.onRightWall = true;
                    }
                } else {
                    if (entityComp.worldBounds.Center().y > otherComp.worldBounds.Center().y) {
                        location.loc.y += overlap.Height() * massRatio;
                        movement.velocity.y +=
                            (otherComp.lastVelocity.y - movement.velocity.y) * massRatio;
                        entityComp.onCeiling = true;
                    } else {
                        location.loc.y -= overlap.Height() * massRatio;
                        movement.velocity.y +=
                            (otherComp.lastVelocity.y - movement.velocity.y) * massRatio;
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
                        resolveCollision(em, otherComp, dynamicComp, dynamicEntity);

                        if (otherComp.doesDamage) applyDamage(em, dynamicEntity);

                        // Handle collectables
                        if (dynamicEntity.template HasComponent<InputComponent>() && otherComp.isCollectable) {
                            em.RemoveEntity(otherPair.first);

                            auto& scoreStore = em.template GetComponentStorage<ScoreComponent>();
                            for (auto& scorePair : scoreStore) {
                                scorePair.second.score += 6;
                            }
                        }
                    }
                }
            }
        }

        return kContinue;
    }
};

}  // namespace ECSEngine
