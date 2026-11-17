#pragma once


#include "../../model/Input.hpp"

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

