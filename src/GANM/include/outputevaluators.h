#include <iostream>

template <typename T>
T squared_difference(T a, T b) {
    T diff = a - b;
    return diff * diff;
}