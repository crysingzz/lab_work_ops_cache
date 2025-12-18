#pragma once
#include <string>

template<typename Key, typename Value>
class ICache {
public:
    virtual ~ICache() = default;
    virtual void set(const Key& key, const Value& value) = 0;
    virtual Value get(const Key& key) = 0;
    virtual bool contains(const Key& key) const = 0;
    virtual void clear() = 0;
    virtual std::size_t size() const = 0;
};



