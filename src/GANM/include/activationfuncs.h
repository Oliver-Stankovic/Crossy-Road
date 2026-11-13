#pragma once

#include <algorithm>

double ReLU(double x) {
    return std::max(0.0, x);
}

double Sigmoid(double x){
    return 1.0 / (1.0 + exp(-x));
}
