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
        if (cfg_.populationSize < 2)
            throw invalid_argument("populationSize must be >= 2");
        if (cfg_.eliteCount >= cfg_.populationSize)
            throw invalid_argument("eliteCount must be < populationSize");
        if (cfg_.tournamentSize == 0)
            throw invalid_argument("tournamentSize must be >= 1");
    }

    Candidate run() {
        initialisePopulation();
        evaluateAll(population_);
        sortByFitness(population_);
        best_ = population_.front();

        for (size_t gen = 1; gen <= cfg_.generations; ++gen) {
            population_ = nextGeneration();  
            evaluateAll(population_);
            sortByFitness(population_);
            if (population_.front().fitness < best_.value().fitness)
                best_ = population_.front();  
            
            if (gen % cfg_.reportEvery == 0){ //Report every reportEvery generations
                cout << "Best candidate fitness for generation " << gen << " :" << best_.value().fitness << "\n";
            }
        }

        return population_.front();
    }

private:

    // NOVO - stvaranje populacije san zaminila sa funkcijon koja svakoj mrezi daje seed
    void initialisePopulation() {
        population_.clear();

        for (size_t i = 0; i < cfg_.populationSize; ++i) {
            NN_Config config;
            config.seed = rng_();

            population_.push_back(Candidate{NeuralNetwork(config), 0.0});
        }
}

    //--------------------------------------------------
    //Evaluira sve jedinke u populaciji na temelju odredene
    //funckije kvalitete
    //--------------------------------------------------
    void evaluateAll(vector<Candidate>& pop) const {
        for (Candidate& cand : pop) {
            BinarnaFunkcija(cand, cfg_.input, cfg_.target);
            //kod je prebacen u "outputevaluators.h"
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
        uniform_int_distribution<size_t> pick(0, population_.size() - 1);
        const Candidate* winner = &population_[pick(rng_)];
        for (size_t i = 1; i < cfg_.tournamentSize; ++i) {
            const Candidate* challenger = &population_[pick(rng_)];
            if (challenger->fitness < winner->fitness) winner = challenger;
        }
        return *winner;
    }

    //----------------------------------------------
    //Za svaki parametar u dijetetu baci novcic i na temelju toga odluci od kojeg
    //ce roditelja prekopirati parametar
    //----------------------------------------------
    NeuralNetwork crossover(const NeuralNetwork& a, const NeuralNetwork& b){
        //NN_Config nnconfig;
        vector<double> a_params = a.getParameters();
        vector<double> b_params = b.getParameters();

        NeuralNetwork child{NN_Config()};
        vector<double> child_params;

        for (size_t i = 0; i < a_params.size(); ++i){ //svi imaju isti broj parametra
            uniform_int_distribution<size_t> coinflip(0, 1);
            if (coinflip(rng_) == 0)
                child_params.push_back(a_params[i]);
            else
                child_params.push_back(b_params[i]);
        }

        child.setParameters(child_params);
        return child;
    }

    //---------------------------------------------
    //Promijeni vrijednost parametra za postotak tezine (za sad),
    //prema gore ako je checkval > (1 - vjerojatnosti mutacije) te 
    //prema dolje ako je checkval < vjerojatnosti mutacije
    //---------------------------------------------
    void mutate(NeuralNetwork& network) {
        auto params = network.getParameters();

        uniform_real_distribution<double> mutationcheck(0.0, 1.0);

        for (double& weight : params){
            double checkval = mutationcheck(rng_);

            if (checkval > (1 - cfg_.mutationRate))
                weight += weight * cfg_.mutationSigma;
            else if (checkval < cfg_.mutationRate)
                weight -= weight * cfg_.mutationSigma;

        }

        network.setParameters(params);
}

    // ---- Step 5: build the next generation ---------------------------------
    // Precondition: population_ is sorted best-first (so elites are at front).
    vector<Candidate> nextGeneration() {
        vector<Candidate> next;
        next.reserve(cfg_.populationSize);

        // Elitism: copy the best individuals unchanged so progress is never lost.
        for (size_t i = 0; i < cfg_.eliteCount; ++i)
            next.push_back(population_[i]);

        // Fill the rest with offspring.
        uniform_real_distribution<double> coin(0.0, 1.0);
        while (next.size() < cfg_.populationSize) {
            const Candidate& p1 = tournamentSelect();
            const Candidate& p2 = tournamentSelect();

            Candidate child{NN_Config()};
            // With probability crossoverRate recombine; otherwise clone parent 1.
            child.neuralnetwork = (coin(rng_) < cfg_.crossoverRate)
                              ? crossover(p1.neuralnetwork, p2.neuralnetwork)
                              : p1.neuralnetwork;
            mutate(child.neuralnetwork);
            next.push_back(move(child));
        }
        return next;
    }

    GA_Config cfg_;
    mt19937 rng_;  // Mersenne Twister: fast, high quality, seedable
    vector<Candidate> population_;
    optional<Candidate> best_;
};