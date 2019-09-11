#ifndef META_HELPERS_HPP
#define META_HELPERS_HPP
#include <type_traits>

namespace Stealth::Engine {
    template <typename T>
    using removeCVRef = std::remove_cv_t<std::remove_reference_t<T>>;
} // Stealth::Engine

#endif // META_HELPERS_HPP
