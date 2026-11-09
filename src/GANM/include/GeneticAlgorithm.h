#pragma once

#include <random>
#include <optional>

#include "configs.h"
#include "candidate.h"

class GeneticAlgorithm{
public: 
    GeneticAlgorithm( Config config)
        :  cfg_(config), rng_(config.seed) {
        // Defensive checks: catch settings that would silently break the loop.
        if (cfg_.populationSize < 2)
            throw std::invalid_argument("populationSize must be >= 2");
        if (cfg_.eliteCount >= cfg_.populationSize)
            throw std::invalid_argument("eliteCount must be < populationSize");
        if (cfg_.tournamentSize == 0)
            throw std::invalid_argument("tournamentSize must be >= 1");
    }


private:

    void initialisePopulation() {
        NN_Config nnconfig;
        for (size_t i = 0; i<cfg_.populationSize; ++i){
            population_.push_back(Candidate{NeuralNetwork(nnconfig), 0});
        }
    }

    void evaluateAll(std::vector<Candidate>& pop) const {
        for (Candidate& ind : pop) ind.fitness = ind.neuralnetwork
    }

    Config cfg_;
    mt19937 rng_;  // Mersenne Twister: fast, high quality, seedable
    vector<Candidate> population_;
    std::optional<Candidate> best_;
};