#pragma once

#include <vector>
#include <string>
#include <functional>
#include <cstdint>

using Genome = std::vector<double>;

struct Individual {
    Genome genes;
    double fitness = 0.0;  // objective value; LOWER IS BETTER (minimisation)
};

// All tunable knobs in one place. Changing these is the best way to learn how
// a GA behaves (try the experiments listed at the end of the file).
struct Config {
    std::size_t populationSize = 100;  // individuals per generation
    std::size_t generations    = 300;  // how long to evolve
    std::size_t tournamentSize = 5;    // selection pressure (bigger = greedier)
    std::size_t eliteCount     = 2;    // best individuals copied unchanged
    double crossoverRate       = 0.9;  // probability a pair is recombined
    double mutationRate        = 0.01;  // probability PER GENE of mutating
    double mutationSigma       = 0.15; // mutation step as a fraction of range
    double blendAlpha          = 0.75;  // BLX-alpha: how far children may stray
    std::uint32_t seed         = 42;   // RNG seed (reproducibility)
    std::size_t reportEvery    = 25;   // print stats every N generations
};

// A problem = search space + objective function.
struct Problem {
    std::string name;
    std::size_t dimensions;
    double lower;  // every gene is confined to [lower, upper]
    double upper;
    std::function<double(const Genome&)> evaluate;
};