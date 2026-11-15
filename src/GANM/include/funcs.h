#pragma once

#include <algorithm>

#include "candidate.h"

//-----------------------------------------------------------------------------------------
//aktivacijske funkcije
//-----------------------------------------------------------------------------------------
double ReLU(double x) {
    return std::max(0.0, x);
}

double Sigmoid(double x){
    return 1.0 / (1.0 + exp(-x));
}

//-----------------------------------------------------------------------------------------
//evaluacijske funkcije
//-----------------------------------------------------------------------------------------
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
void SredinaKvadratnihRazlika(Candidate& cand, vector<vector<double>> input, vector<vector<double>> target){
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

void BinarnaFunkcija(Candidate& cand, vector<vector<double>> input, vector<vector<double>> target){
    double ukupnaPogreska = 0.0;
    for (size_t i = 0; i < input.size(); ++i) {
        vector<double> output = cand.neuralnetwork.forward(input[i]);
        double inputPogreska = 0.0;
        for (size_t j = 0; j < output.size(); ++j){
            double odluka = (output[j] >= 0.5) ? 1 : 0;
            inputPogreska += squared_difference(odluka, target[i][j]);
        }
        ukupnaPogreska += inputPogreska / output.size();
    }
    cand.fitness = ukupnaPogreska / input.size();
}



//-------------------------
//Dosl samo static cast iz float u double
//------------------------
vector<double> pretvoriZrake(const vector<float>& zrake) {
    vector<double> ulazi;
    for(float zraka : zrake) {
        ulazi.push_back(static_cast<double>(zraka));
    }
    return ulazi;
}

//-------------------------------------
//Odabir najvece vrijednosti medu 5 izlazna
//-------------------------------------
Input odaberiPotez(const vector<double>& izlazi) {
    const Input potezi[] = {
        Input::None, Input::Left, Input::Right, Input::Up, Input::Down
    };

    size_t najboljiIndeks = 0;
    for(size_t i = 1; i < izlazi.size(); ++i) {
        if (izlazi[i] > izlazi[najboljiIndeks]) najboljiIndeks = i;
    }
    return potezi[najboljiIndeks];
}