#pragma once

#include <type_traits>

namespace Plteen {
    template<typename T> struct SuperType { using type = long long; };
    template<> struct SuperType<double> { using type = double; };
    template<> struct SuperType<float> { using type = double; };
    
    template<typename Fl> struct LengthType { using type = float; };
    template<> struct LengthType<double> { using type = double; };

    template<typename T>
    using super_t = typename SuperType<T>::type;

    template<typename T>
    using real_t = typename std::enable_if<std::is_arithmetic<T>::value>::type;

    template<typename T>
    using fixnum_t = typename std::enable_if<std::is_integral<T>::value>::type;

    template<typename T>
    using flonum_t = typename std::enable_if<std::is_floating_point<T>::value>::type;

    template<typename T, typename V = T>
    using real_if_t = typename std::enable_if<std::is_arithmetic<T>::value, V>::type;

    template<typename T, typename V = T>
    using fixnum_if_t = typename std::enable_if<std::is_integral<T>::value, V>::type;

    template<typename T, typename V = T>
    using flonum_if_t = typename std::enable_if<std::is_floating_point<T>::value, V>::type;
}
