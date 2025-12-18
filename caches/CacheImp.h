#pragma once
#include "ICache.h"
#include <vector>
#include <unordered_map>

template<typename Key, typename Value>
class ClockCache : public ICache<Key, Value> {
private:
    struct CacheEntry {
        Key key;
        Value value;
        bool flag;
        bool valid;
    };
    
    std::vector<CacheEntry> entries;
    std::unordered_map<Key, size_t> key_map;
    size_t max_capacity;
    size_t clock_hand;
    size_t current_size;

public:
    ClockCache(size_t capacity = 10) : max_capacity(capacity), clock_hand(0), current_size(0) {
        if (max_capacity == 0) max_capacity = 1;
        entries.resize(max_capacity);
    }

    void set(const Key& key, const Value& value) override {
        auto it = key_map.find(key);
        if (it != key_map.end()) {
            size_t index = it->second;
            entries[index].value = value;
            entries[index].flag = true;
            return;
        }


        if (current_size < max_capacity) {
            entries[current_size] = {key, value, true, true};
            key_map[key] = current_size;
            current_size++;
            return;
        }
        size_t victim_index = findNeeded();
        Key old_key = entries[victim_index].key;
        key_map.erase(old_key);
        entries[victim_index] = {key, value, true, true};
        key_map[key] = victim_index;
    }

    Value get(const Key& key) override {
        auto it = key_map.find(key);
        if (it != key_map.end()) {
            size_t index = it->second;
            entries[index].flag = true;
            return entries[index].value;
        }
        return Value();
    }

    bool contains(const Key& key) const override {
        return key_map.find(key) != key_map.end();
    }

    void clear() override {
        key_map.clear();
        for (size_t i = 0; i < max_capacity; i++) {
            entries[i].valid = false;
            entries[i].flag = false;
        }
        current_size = 0;
        clock_hand = 0;
    }

    size_t size() const override {
        return current_size;
    }

private:
    size_t findNeeded() {
        while (true) {
            if (entries[clock_hand].flag == false) {
                size_t victim = clock_hand;
                clock_hand = (clock_hand + 1) % max_capacity;
                return victim;
            } else {
                entries[clock_hand].flag = false;
                clock_hand = (clock_hand + 1) % max_capacity;
            }
        }
    }
};