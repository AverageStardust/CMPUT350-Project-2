#pragma once

#include <assert.h>

#include <cstddef>
#include <cstdint>
#include <limits>
#include <vector>

namespace ECSEngine {

using Ticket = uint64_t;
static constexpr int64_t NullTicket = std::numeric_limits<uint64_t>::max();

struct SplitTicket {
    uint32_t index;
    int32_t generation;
};

template <typename T>
struct StorageSlot {
    T value;
    // >0 when live, <=0 when free, absolute value increases with each new value
    int32_t generation = 0;
};

// generic iterable slot map
// uses 32-bits per ticket as an index, and the remaining 32-bits as a generation number
template <typename T>
class Storage {
    std::vector<StorageSlot<T>> slots;
    std::vector<uint32_t> freeSlots;

public:
    // Referanced from https://www.internalpointers.com/post/writing-custom-iterators-modern-cpp
    struct Iterator {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;

        Iterator(Storage<T>& storage, int index) : mStorage(storage), mIndex(index) {
            // skip past empty slots
            int size = mStorage.slots.size();
            while (mStorage.slots[mIndex].generation <= 0 && mIndex < size) mIndex++;
        }

        reference operator*() const { return mStorage.slots[mIndex]; }
        pointer operator->() { return *mStorage.slots[mIndex]; }

        Iterator& operator++() {
            mIndex++;

            // skip past empty slots
            int size = mStorage.slots.size();
            while (mStorage.slots[mIndex].generation <= 0 && mIndex < size) mIndex++;

            return *this;
        }

        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        friend bool operator==(const Iterator& a, const Iterator& b) {
            return a.mIndex == b.mIndex && a.mStorage == b.mStorage;
        };
        friend bool operator!=(const Iterator& a, const Iterator& b) {
            return a.mIndex != b.mIndex || a.mStorage != b.mStorage;
        };

    private:
        Storage<T>& mStorage;
        int mIndex;
    };

    Ticket Store(const T& value) { return storeInternal(value); }
    Ticket Store(T&& value) { return storeInternal(std::move(value)); }

    void Remove(Ticket ticket) {
        SplitTicket splitTicket = cutTicket(ticket);
        if (!isValid(splitTicket)) return;

        slots[splitTicket.index] = StorageSlot<T>{.value = T{}, .generation = -splitTicket.index};

        freeSlots.push_back(splitTicket.index);
    }

    T& Get(Ticket ticket) {
        SplitTicket splitTicket = cutTicket(ticket);
        assert(isValid(splitTicket));

        return slots[splitTicket.index].value;
    }

    const T& Get(Ticket ticket) const { return Get(ticket); }
    T& operator[](Ticket ticket) { return Get(ticket); };
    const T& operator[](Ticket ticket) const { return Get(ticket); };

    bool IsValid(Ticket ticket) const { return isValid(cutTicket(ticket)); }

    uint32_t Size() const { return slots.size() - freeSlots.size(); }

    void Reserve(uint32_t capacity) { slots.reserve(capacity); }

    Iterator begin() { return Iterator(this, 0); }

    Iterator end() { return Iterator(this, slots.size()); }

private:
    template <typename U>  // U is either T or T&
    Ticket storeInternal(U&& value) {
        if (freeSlots.size() == 0) {
            // fill new slot
            slots.push_back(StorageSlot<T>{.value = value, .generation = 1});

            return slots.size() - 1;
        } else {
            // recycle slot
            uint32_t index = freeSlots[freeSlots.size() - 1];
            freeSlots.pop_back();

            // negate since it's currently freed (generation <= 0)
            int32_t generation = -slots[index].generation;

            // increment generation
            slots[index] =
                StorageSlot<T>{.value = std::forward<U>(value), .generation = generation + 1};

            return joinTicket(SplitTicket{.index = index, .generation = generation});
        }
    }

    bool isValid(SplitTicket splitTicket) const {
        return (splitTicket.index < slots.size()) &&
               slots[splitTicket.index].generation == splitTicket.generation;
    }

    SplitTicket cutTicket(Ticket ticket) {
        return SplitTicket{
            .index = (uint32_t)(ticket),
            .generation = (int32_t)(ticket >> 32),
        };
    }

    Ticket joinTicket(SplitTicket splitTicket) {
        return (uint64_t)splitTicket.index + (((uint64_t)splitTicket.generation) << 32);
    }
};

}  // namespace ECSEngine
