#ifndef MEMORY_BIDIRECTIONAL_MAP_HPP
#define MEMORY_BIDIRECTIONAL_MAP_HPP
#include <unordered_map>

namespace Stealth::Engine {
    template <typename Key, typename Value>
    class BidirectionalMap {
    public:
        constexpr bool containsKey(const Key& key) const noexcept {
            return m1.count(key) == 1;
        }

        constexpr bool containsValue(const Value& value) const noexcept {
            return m2.count(value) == 1;
        }

        const Value& valueAt(const Key& key) const {
            return *m1.at(key);
        }

        const Key& keyAt(const Value& value) const {
            return *m2.at(value);
        }

        void set(Key key, Value value) {
            const auto& [keyIter, insertedKey] = m1.insert_or_assign(std::move(key), nullptr);
            const auto& [valIter, insertedVal] = m2.insert_or_assign(std::move(value), &keyIter->first);
            m1[key] = &valIter->first;
        }
    protected:
        std::unordered_map<Key, const Value*> m1;
        std::unordered_map<Value, const Key*> m2;
    };
} // Stealth::Engine

#endif // MEMORY_BIDIRECTIONAL_MAP_HPP
