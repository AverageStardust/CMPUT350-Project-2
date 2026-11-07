#include "Component.h"

namespace ECSEngine {

template <typename T>
Ticket ComponentStorage<T>::Store(T&& component) {
    liveSlots++;

    if (freeSlots.size() == 0) {
        // fill new slot
        slots.push_back(component);
        isSlotsLive.push_back(true);

        return slots.size() - 1;
    } else {
        // recycle slot
        size_t ticket = freeSlots[freeSlots.size() - 1];
        freeSlots.pop_back();

        slots[ticket] = std::forward<T>(component);
        isSlotsLive[ticket] = true;

        return ticket;
    }
}

template <typename T>
void ComponentStorage<T>::Remove(Ticket ticket) {
    if (!IsValid(ticket)) return;

    isSlotsLive[ticket] = false;

    slots[ticket] = T{};  // replace causing deconstruction

    freeSlots.push_back(ticket);
}

template <typename T>
T& ComponentStorage<T>::operator[](Ticket ticket) {
    assert(IsValid(ticket));
    return slots[ticket];
}

template <typename T>
bool ComponentStorage<T>::IsValid(Ticket ticket) const {
    return (ticket < slots.size()) && isSlotsLive[ticket];
}

template <typename T>
size_t ComponentStorage<T>::Size() const {
    return liveSlots;
}

template <typename T>
void ComponentStorage<T>::Reserve(size_t capacity) {
    slots.reserve(capacity);
    isSlotsLive.reserve(capacity);
}

}  // namespace ECSEngine
