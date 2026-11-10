#pragma once

#include <iostream>
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

    Candidate run() {
        initialisePopulation();
        evaluateAll(population_);
        sortByFitness(population_);

        population_ = nextGeneration();
        evaluateAll(population_);
        sortByFitness(population_);

        return population_.front();
    }

private:
    // izvolte
    vector<Candidate> nextGeneration();


    // NOVO - stvaranje populacije san zaminila sa funkcijon koja svakoj mrezi daje seed
    void initialisePopulation() {
    population_.clear();

    for (size_t i = 0; i < cfg_.populationSize; ++i) {
        NN_Config config;
        config.seed = rng_();

        population_.push_back(Candidate{NeuralNetwork(config), 0.0});
    }
}

    // NOVO - ocjenjivanje svake mreze san zaminila sa prosjecnon pogreskom
    void evaluateAll(vector<Candidate>& pop) const {
    for (Candidate& cand : pop) {
        double ukupnaPogreska = 0.0;

        for (size_t i = 0; i < cfg_.examples.size(); ++i) {
            auto output = cand.neuralnetwork.forward(cfg_.examples[i]);
            ukupnaPogreska += squared_difference(output[0], cfg_.zeljeniIzlazi[i]);
        }

        cand.fitness = ukupnaPogreska / cfg_.examples.size();
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
            } else{
                child_layers.push_back(b.getLayers()[i]);
            }
        }
        child.setLayers(child_layers);
        return child;
    }

    // NOVO - promijeni jedan nasumicno odabran parametar
    void mutate(NeuralNetwork& network) {
        auto parametri = network.getParameters();

        uniform_int_distribution<size_t> odaberiParametar(0, parametri.size() - 1);
        uniform_real_distribution<double> odaberiPromjenu(-0.5, 0.5);

        size_t indeks = odaberiParametar(rng_);
        parametri[indeks] += odaberiPromjenu(rng_);

        network.setParameters(parametri);
}

    GA_Config cfg_;
    mt19937 rng_;  // Mersenne Twister: fast, high quality, seedable
    vector<Candidate> population_;
    optional<Candidate> best_;
};