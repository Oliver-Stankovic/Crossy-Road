#pragma once

#include <iostream>
#include "candidate.h"

template <typename T>
T squared_difference(T a, T b) {
    T diff = a - b;
    return diff * diff;
}

template <typename T>
T difference_of_squares(T a, T b) {
    return (a * a) - (b * b);
}


//------------------------------------------
//Racuna srednju vrijednost srednjih vrijednost
//kvadrirane razlike izmedu izlaza mreze i trazene
//vrijednosti
//------------------------------------------
void SrednjaSrednjih(Candidate& cand, vector<vector<double>> input, vector<vector<double>> target){
    double ukupnaPogreska = 0.0;
    for (size_t i = 0; i < input.size(); ++i) {
        vector<double> output = cand.neuralnetwork.forward(input[i]);
        double inputPogreska = 0.0;
        for (size_t j = 0; j < output.size(); ++j){
            inputPogreska += squared_difference(output[j], target[i][j]);
        }
        ukupnaPogreska += inputPogreska / output.size();
    }
    cand.fitness = ukupnaPogreska / input.size();
}
    