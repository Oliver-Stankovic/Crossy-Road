#pragma once

#include <vector>
#include <cstdint>

using namespace std;

struct NN_Config{
    vector<size_t> architecture = {2, 4, 1};
    uint32_t seed = 42;
    double lowerinit = -1.0;
    double upperinit = 1.0;
};

struct GA_Config{
    size_t populationSize = 15;  // individuals per generation
    size_t generations    = 50;  // how long to evolve
    size_t tournamentSize = 5;    // selection pressure (bigger = greedier)
    size_t eliteCount     = 2;    // best individuals copied unchanged
    double crossoverRate       = 0.9;  // probability a pair is recombined
    double mutationRate        = 0.01;  // probability PER GENE of mutating
    double mutationSigma       = 0.05; // mutation step as a fraction of range
    double blendAlpha          = 0.75;  // BLX-alpha: how far children may stray
    uint32_t seed         = 98;   // RNG seed (reproducibility)
    size_t reportEvery    = 5;   // print stats every N generations
    
    //
    vector<vector<double>> input = {
        {0.0, 0.0},
        {0.0, 1.0},
        {1.0, 0.0},
        {1.0, 1.0}
    };
    
    vector<vector<double>> target = {{0.0}, {0.0}, {0.0}, {1.0}};
};


