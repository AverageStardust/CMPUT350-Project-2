#pragma once

#include <cassert>

#include "Entity.h"
#include "Storage.h"

namespace ECSEngine {
template <typename... Components>
class EntityManager {
public:
    [[nodiscard]] EntityID CreateEntity() {
        EntityID id = mEntities.Store(Entity<Components...>{});
        mEntities[id].mID = id;
        return id;
    }

    Entity<Components...>& GetEntity(EntityID entity) {
        return mEntities.Get(entity);
    }

    void RemoveEntity(EntityID entity) {
        (RemoveComponent<Components>(entity), ...);
        mEntities.Remove(entity);
    }

    template <typename T>
    void AddComponent(EntityID entity, T component) {
        Ticket componentID = GetComponentStorage<T>().Store(component);
        GetEntity(entity).template GetComponetID<T>() = componentID;
    }

    template <typename T>
    void RemoveComponent(EntityID entity) {
        Ticket componentID = GetEntity(entity).template GetComponetID<T>();
        if (componentID == NullComponent) return;

        GetComponentStorage<T>().Remove(componentID);
    }

    template <typename T>
    bool HasComponent(Entity<Components...> entity) const {
        return entity.template GetComponetID<T>() != NullComponent;
    }

    template <typename T>
    bool HasComponent(EntityID entity) const {
        return HasComponent(GetEntity(entity));
    }

    template <typename T>
    T& GetComponent(Entity<Components...> entity) {
        Ticket componentID = entity.template GetComponetID<T>();
        return GetComponentStorage<T>().Get(componentID);
    }

    template <typename T>
    T& GetComponent(EntityID entity) {
        return GetComponent<T>(GetEntity(entity));
    }

    template <typename T>
    Storage<T>& GetComponentStorage() {
        return std::get<Storage<T>>(mComponents);
    }

    Storage<Entity<Components...>>::Iterator cbegin() const { return mEntities.begin(); }
    Storage<Entity<Components...>>::Iterator cend() const { return mEntities.end(); }
    Storage<Entity<Components...>>::Iterator begin() { return mEntities.begin(); }
    Storage<Entity<Components...>>::Iterator end() { return mEntities.end(); }

private:
    Storage<Entity<Components...>> mEntities;
    std::tuple<Storage<Components>...> mComponents;
};

}  // namespace ECSEngine
