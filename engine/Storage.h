#pragma once

#include <assert.h>

#include <cstddef>
#include <limits>
#include <vector>

namespace ECSEngine {

using Ticket = size_t;
static constexpr size_t NullTicket = std::numeric_limits<size_t>::max();

template <typename T>
class Storage {
    std::vector<T> slots;
    std::vector<size_t> freeSlots;
    std::vector<bool> isSlotsLive;

public:
    Ticket Store(const T& component) { return StoreInternal(component); }
    Ticket Store(T&& component) { return StoreInternal(std::move(component)); }

    void Remove(Ticket ticket) {
        if (!IsValid(ticket)) return;

        isSlotsLive[ticket] = false;

        slots[ticket] = T{};  // replace causing deconstruction

        freeSlots.push_back(ticket);
    }

    T& Get(Ticket ticket) {
        assert(IsValid(ticket));
        return slots[ticket];
    }

    const T& Get(Ticket ticket) const { return Get(ticket); }
    T& operator[](Ticket ticket) { return Get(ticket); };
    const T& operator[](Ticket ticket) const { return Get(ticket); };

    bool IsValid(Ticket ticket) const { return (ticket < slots.size()) && isSlotsLive[ticket]; }

    size_t Size() const { return slots.size() - freeSlots.size(); }

    void Reserve(size_t capacity) {
        slots.reserve(capacity);
        isSlotsLive.reserve(capacity);
    }

private:
    template <typename U>  // U is either T or T&
    Ticket StoreInternal(U&& component) {
        if (freeSlots.size() == 0) {
            // fill new slot
            slots.push_back(component);
            isSlotsLive.push_back(true);

            return slots.size() - 1;
        } else {
            // recycle slot
            size_t ticket = freeSlots[freeSlots.size() - 1];
            freeSlots.pop_back();

            slots[ticket] = std::forward<U>(component);
            isSlotsLive[ticket] = true;

            return ticket;
        }
    }
};

}  // namespace ECSEngine
