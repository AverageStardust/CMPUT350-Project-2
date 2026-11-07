#pragma once

#include <vector>
#include <assert.h>
#include <cstddef>

using Ticket = size_t;

template <typename T>
class Registry {
    std::vector<T> slots;
    std::vector<size_t> deletedSlots;
    std::vector<bool> isSlotsLive;
    size_t liveSlots = 0;

public:

    Ticket store(T& component);
    Ticket store(T&& component);

    void remove(Ticket ticket);

    T& operator[](Ticket ticket);
    const T& operator[](Ticket ticket) const;

    bool valid(Ticket ticket) const;

    size_t size() const;

    void reserve(size_t capacity);
};

template <typename T>
Ticket Registry<T>::store(T&& component) {
    liveSlots++;

    if (deletedSlots.size() == 0) {
        // fill new slot
        slots.push_back(component);
        isSlotsLive.push_back(true);

        return slots.size() - 1;
    } else {
        // recycle slot
        size_t ticket = deletedSlots[deletedSlots.size() - 1];
        deletedSlots.pop_back();

        slots[ticket] = std::forward<T>(component);
        isSlotsLive[ticket] = true;

        return ticket;
    }
}

template <typename T>
void Registry<T>::remove(Ticket ticket) {
    if (!valid(ticket)) return;

    isSlotsLive[ticket] = false;

    slots[ticket] = T{}; // replace causing deconstruction

    deletedSlots.push_back(ticket);
}

template <typename T>
T& Registry<T>::operator[](Ticket ticket) {
    assert(valid(ticket));
    return slots[ticket];
}

template <typename T>
bool Registry<T>::valid(Ticket ticket) const {
    return (ticket < slots.size()) && isSlotsLive[ticket];
}

template <typename T>
size_t Registry<T>::size() const {
    return liveSlots;
}

template <typename T>
void Registry<T>::reserve(size_t capacity) {
    slots.reserve(capacity);
    isSlotsLive.reserve(capacity);
}
