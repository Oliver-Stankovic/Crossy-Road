#pragma once

#include <stdexcept>
#include <cmath>

#include "structs.h"

constexpr double kPi = 3.14159265358979323846;

// Sphere: a smooth bowl. Easy; one minimum. Good for sanity checks.
inline Problem sphere(std::size_t dims = 10) {
    return {"sphere", dims, -5.12, 5.12, [](const Genome& x) {
                double sum = 0.0;
                for (double xi : x) sum += xi * xi;
                return sum;
            }};
}

// Rastrigin: a bowl covered in a grid of local minima. Hard for hill-climbers
// because they get stuck; a population can explore several basins at once.
inline Problem rastrigin(std::size_t dims = 10) {
    return {"rastrigin", dims, -5.12, 5.12, [](const Genome& x) {
                double sum = 10.0 * static_cast<double>(x.size());
                for (double xi : x) sum += xi * xi - 10.0 * std::cos(2.0 * kPi * xi);
                return sum;
            }};
}

// Ackley: nearly flat outer region with a narrow funnel at the centre.
inline Problem ackley(std::size_t dims = 10) {
    return {"ackley", dims, -32.768, 32.768, [](const Genome& x) {
                const double n = static_cast<double>(x.size());
                double sumSq = 0.0, sumCos = 0.0;
                for (double xi : x) {
                    sumSq += xi * xi;
                    sumCos += std::cos(2.0 * kPi * xi);
                }
                return -20.0 * std::exp(-0.2 * std::sqrt(sumSq / n)) -
                       std::exp(sumCos / n) + 20.0 + std::exp(1.0);
            }};
}

inline Problem byName(const std::string& name) {
    if (name == "sphere") return sphere();
    if (name == "rastrigin") return rastrigin();
    if (name == "ackley") return ackley();
    throw std::invalid_argument("unknown problem: " + name);
}

