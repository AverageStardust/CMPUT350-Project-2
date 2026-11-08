#pragma once

#include <string>
#include <tuple>

#include "Storage.h"

// An entity can have any component it is templated with,
// but does not have these installed by default.
// This is a base class for all entities in the ECS system.

namespace ECSEngine {

using EntityID = Ticket;
static constexpr size_t NullComponent = std::numeric_limits<Ticket>::max();

template <typename T>
struct ComponentSlot {
    Ticket componentID;
};

template <typename... Components>
class Entity {
public: // TODO: MAKE PRIVATE
    Entity() = default;
    Entity(const std::string& name) : mName(name) {}

    std::string mName;
    std::tuple<ComponentSlot<Components>...> mComponents{
        ComponentSlot<Components>{NullComponent}...};

    template <typename T>
    Ticket& GetComponetID() {
        return std::get<ComponentSlot<T>>(mComponents).componentID;
    }

    template <typename... Any>
    friend class EntityManager;
};

}  // namespace ECSEngine
