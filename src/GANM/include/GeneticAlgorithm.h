#pragma once

#include <random>
#include <optional>
#include <algorithm>

#include "configs.h"
#include "candidate.h"
#include "outputevaluators.h"

class GeneticAlgorithm{
public: 
    GeneticAlgorithm(GA_Config config)
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
        //NN_Config nnconfig;
        for (size_t i = 0; i<cfg_.populationSize; ++i){
            population_.push_back(Candidate{NeuralNetwork(NN_Config()), 0});
        }
    }

    void evaluateAll(vector<Candidate>& pop) const {
        for (Candidate& cand : pop){
            vector<double> output = cand.neuralnetwork.forward(cfg_.input);
            for (size_t i = 0; i<cfg_.target.size(); i++){
                cand.fitness += squared_difference(output[i], cfg_.target[i]);
            }
        }
    }

    static void sortByFitness(vector<Candidate>& pop) {
        sort(pop.begin(), pop.end(),
                  [](const Candidate& a, const Candidate& b) {
                      return a.fitness < b.fitness;  // best (lowest) first
                  });
    }

    //------------------------------------------------
    //Bira najboljeg od "tournamentSize" nasumicno izabranih kandidata
    //------------------------------------------------
    const Candidate& tournamentSelect() {
        std::uniform_int_distribution<std::size_t> pick(0, population_.size() - 1);
        const Candidate* winner = &population_[pick(rng_)];
        for (std::size_t i = 1; i < cfg_.tournamentSize; ++i) {
            const Candidate* challenger = &population_[pick(rng_)];
            if (challenger->fitness < winner->fitness) winner = challenger;
        }
        return *winner;
    }

    //----------------------------------------------
    //Za svaki sloj u dijetetu baci novcic i na temelju toga odluci od kojeg
    //ce roditelja prekopirati sloj
    //----------------------------------------------
    NeuralNetwork crossover(const NeuralNetwork& a, const NeuralNetwork& b){
        //NN_Config nnconfig;
        NeuralNetwork child{NN_Config()};
        vector<vector<Neuron>> child_layers;
        for (size_t i = 0; i < a.getLayers().size(); ++i){ //svi imaju isti broj layera (govoriti "slojeva" pred Cupicem)
            uniform_int_distribution<std::size_t> coinflip(0, 1);
            if (coinflip(rng_) == 0){
                child_layers.push_back(a.getLayers()[i]);
            }else{
                child_layers.push_back(b.getLayers()[i]);
            }
        }
        child.setLayers(child_layers);
        return child;
    }

    void mutate(NeuralNetwork& nnetwork){
        uniform_real_distribution<double> coin(0.0, 1.0);
    }

    GA_Config cfg_;
    mt19937 rng_;  // Mersenne Twister: fast, high quality, seedable
    vector<Candidate> population_;
    optional<Candidate> best_;
};