#pragma once

#include <vector>
#include <cstdint>

using namespace std;

struct NN_Config{
    vector<size_t> architecture = {3, 6, 2};
    uint32_t seed = 42;
    double lowerinit = -10.0;
    double upperinit = 10.0;
};

struct GA_Config{
    size_t populationSize = 50;  // individuals per generation
    size_t generations    = 200;  // how long to evolve
    size_t tournamentSize = 5;    // selection pressure (bigger = greedier)
    size_t eliteCount     = 5;    // best individuals copied unchanged
    double crossoverRate       = 0.9;  // probability a pair is recombined
    double mutationRate        = 0.02;  // probability PER GENE of mutating
    double mutationSigma       = 0.15; // mutation step as a percentage of current value
    double blendAlpha          = 0.75;  // BLX-alpha: how far children may stray
    uint32_t seed         = 42;   // RNG seed (reproducibility)
    size_t reportEvery    = 5;   // print stats every N generations
    
    //
    vector<vector<double>> input = {
        {0.0, 0.0, 0.0},
        {1.0, 0.0, 2.0},
        {2.0, 1.0, -4.0},
        {-1.0, -2.0, 6.0},
        {3.0, 2.0, 1.0}
    };
    
    vector<vector<double>> target = {{0.0, 0.0}, {4.0, 0.0}, {-3.0, 0.0}, {10.0, -4.0}, {1.0, 5.5}};
};


