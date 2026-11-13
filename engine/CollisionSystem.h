#pragma once

#include <string>

#include "CameraComponent.h"
#include "CameraSystem.h"
#include "CollisionComponent.h"
#include "EntityManager.h"
#include "InputComponent.h"
#include "LocationComponent.h"
#include "MathUtil.h"
#include "ScoreComponent.h"
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

    void applyScreenShake(EngineView<Components...>* engine, const Rect& overlap,
                          const Point2D& changeInVelocity) {
        Point2D shake;
        if (overlap.Width() < overlap.Height()) {
            shake = Point2D{changeInVelocity.x, 0};
        } else {
            shake = Point2D{0, changeInVelocity.y};
        }

        CameraSystem<Components...>::ApplyScreenShake(engine, shake);
    }

    void resolveCollision(EngineView<Components...>* engine, EntityManager<Components...>& em, CollisionComponent& otherComp,
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

            LocationComponent& location = em.template GetComponent<LocationComponent>(entity);
            MovementComponent& movement = em.template GetComponent<MovementComponent>(entity);

            Point2D changeInVelocity = otherComp.lastVelocity - entityComp.lastVelocity;
            if (entityComp.isCollectable &&
                !otherComp.isCollectable) {  // remove !otherComp.isCollectable for fun times
                changeInVelocity += entityComp.lastVelocity * -0.7f * massRatio;
            }

            if (entity.template HasComponent<InputComponent>()) {
                applyScreenShake(engine, overlap, changeInVelocity * massRatio);
            }

            if (overlap.Width() < overlap.Height()) {
                if (entityComp.worldBounds.Center().x > otherComp.worldBounds.Center().x) {
                    location.loc.x =
                        otherComp.worldBounds.bottomRight.x - entityComp.localBounds.topLeft.x;
                    movement.velocity.x =
                        entityComp.lastVelocity.x + changeInVelocity.x * massRatio;
                    entityComp.onLeftWall = true;
                } else {
                    location.loc.x =
                        otherComp.worldBounds.topLeft.x - entityComp.localBounds.bottomRight.x;
                    movement.velocity.x =
                        entityComp.lastVelocity.x + changeInVelocity.x * massRatio;
                    entityComp.onRightWall = true;
                }
            } else {
                if (entityComp.worldBounds.Center().y > otherComp.worldBounds.Center().y) {
                    location.loc.y =
                        otherComp.worldBounds.bottomRight.y - entityComp.localBounds.topLeft.y;
                    movement.velocity.y =
                        entityComp.lastVelocity.y + changeInVelocity.y * massRatio;
                    entityComp.onCeiling = true;
                } else {
                    location.loc.y =
                        otherComp.worldBounds.topLeft.y - entityComp.localBounds.bottomRight.y;
                    movement.velocity.y =
                        entityComp.lastVelocity.y + changeInVelocity.y * massRatio;
                    entityComp.onGround = true;
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
                        resolveCollision(engine, em, otherComp, dynamicComp, dynamicEntity);

                        if (otherComp.doesDamage) applyDamage(em, dynamicEntity);

                        // Handle collectables
                        if (dynamicEntity.template HasComponent<InputComponent>() &&
                            otherComp.isCollectable) {
                            em.RemoveEntity(otherPair.first);

                            auto& scoreStore = em.template GetComponentStorage<ScoreComponent>();
                            for (auto& scorePair : scoreStore) {
                                scorePair.second.score += 67;
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
