#ifndef MEMORY_BIDIRECTIONAL_MAP_HPP
#define MEMORY_BIDIRECTIONAL_MAP_HPP
#include <unordered_map>

namespace Stealth::Engine {
    template <typename Key, typename Value>
    using BidirectionalMap = std::unordered_map<Key, Value>;
    // template <typename Key, typename Value>
    // class BidirectionalMap {
    // public:
    //
    // protected:
    //     std::unordered_map<Key, Value*> m1;
    //     std::unordered_map<Value, Key*> m2;
    // };
} // Stealth::Engine

#endif // MEMORY_BIDIRECTIONAL_MAP_HPP
