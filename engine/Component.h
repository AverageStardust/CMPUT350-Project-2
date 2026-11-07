#pragma once

#include <vector>

namespace ECSEngine {

#include <assert.h>

#include <cstddef>
#include <vector>

using Ticket = size_t;

template <typename T>
class ComponentStorage {
    std::vector<T> slots;
    std::vector<size_t> freeSlots;
    std::vector<bool> isSlotsLive;
    size_t liveSlots = 0;

public:
    Ticket Store(T& component);
    Ticket Store(T&& component);

    void Remove(Ticket ticket);

    T& operator[](Ticket ticket);
    const T& operator[](Ticket ticket) const;

    bool IsValid(Ticket ticket) const;

    size_t Size() const;

    void Reserve(size_t capacity);
};

}  // namespace ECSEngine
