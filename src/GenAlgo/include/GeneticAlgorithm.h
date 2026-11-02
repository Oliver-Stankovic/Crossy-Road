#pragma once

#include <stdexcept>
#include <random>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include "structs.h"


class GeneticAlgorithm {
public:
    GeneticAlgorithm(Problem problem, Config config)
        : problem_(std::move(problem)), cfg_(config), rng_(config.seed) {
        // Defensive checks: catch settings that would silently break the loop.
        if (cfg_.populationSize < 2)
            throw std::invalid_argument("populationSize must be >= 2");
        if (cfg_.eliteCount >= cfg_.populationSize)
            throw std::invalid_argument("eliteCount must be < populationSize");
        if (cfg_.tournamentSize == 0)
            throw std::invalid_argument("tournamentSize must be >= 1");
    }

    // Main loop. Returns the best individual ever found.
    Individual run() {
        initialisePopulation();
        evaluateAll(population_);
        sortByFitness(population_);
        best_ = population_.front();

        report(0);
        for (std::size_t gen = 1; gen <= cfg_.generations; ++gen) {
            population_ = nextGeneration();   // selection + crossover + mutation
            evaluateAll(population_);
            sortByFitness(population_);
            if (population_.front().fitness < best_.fitness)
                best_ = population_.front();  // (elitism makes this monotone)
            if (gen % cfg_.reportEvery == 0 || gen == cfg_.generations) report(gen);
        }
        return best_;
    }

    const Problem& problem() const { return problem_; }

private:
    // ---- Step 0: create a random starting population ------------------------
    // Uniform sampling over the whole search space maximises initial diversity.
    void initialisePopulation() {
        std::uniform_real_distribution<double> dist(problem_.lower, problem_.upper);
        population_.assign(cfg_.populationSize, Individual{});
        for (Individual& ind : population_) {
            ind.genes.resize(problem_.dimensions);
            for (double& g : ind.genes) g = dist(rng_);
        }
    }

    // ---- Step 1: fitness evaluation ----------------------------------------
    void evaluateAll(std::vector<Individual>& pop) const {
        for (Individual& ind : pop) ind.fitness = problem_.evaluate(ind.genes);
    }

    static void sortByFitness(std::vector<Individual>& pop) {
        std::sort(pop.begin(), pop.end(),
                  [](const Individual& a, const Individual& b) {
                      return a.fitness < b.fitness;  // best (lowest) first
                  });
    }

    // ---- Step 2: selection --------------------------------------------------
    // Tournament selection: draw k random individuals, return the best of them.
    //   k = 1  -> pure random choice (no selection pressure)
    //   k large-> almost always the very best (fast but loses diversity)
    const Individual& tournamentSelect() {
        std::uniform_int_distribution<std::size_t> pick(0, population_.size() - 1);
        const Individual* winner = &population_[pick(rng_)];
        for (std::size_t i = 1; i < cfg_.tournamentSize; ++i) {
            const Individual* challenger = &population_[pick(rng_)];
            if (challenger->fitness < winner->fitness) winner = challenger;
        }
        return *winner;
    }

    // ---- Step 3: crossover (BLX-alpha blend crossover) ---------------------
    // For each gene, take the interval spanned by the two parents' values,
    // widen it by alpha on both sides, and sample the child's gene uniformly
    // from it. Alpha > 0 lets children land OUTSIDE the parents' range, which
    // helps exploration; alpha = 0 only interpolates.
    Genome crossover(const Genome& a, const Genome& b) {
        Genome child(a.size());
        for (std::size_t i = 0; i < a.size(); ++i) {
            const double lo = std::min(a[i], b[i]);
            const double hi = std::max(a[i], b[i]);
            const double d  = hi - lo;
            std::uniform_real_distribution<double> dist(lo - cfg_.blendAlpha * d,
                                                        hi + cfg_.blendAlpha * d);
            child[i] = clampGene(dist(rng_));
        }
        return child;
    }

    // ---- Step 4: mutation (Gaussian) ---------------------------------------
    // Each gene is, with small probability, nudged by normally distributed
    // noise. Without mutation, the population can only recombine what it
    // already has and may converge prematurely to a poor local optimum.
    void mutate(Genome& genes) {
        std::uniform_real_distribution<double> coin(0.0, 1.0);
        const double sigma = cfg_.mutationSigma * (problem_.upper - problem_.lower);
        std::normal_distribution<double> noise(0.0, sigma);
        for (double& g : genes)
            if (coin(rng_) < cfg_.mutationRate) g = clampGene(g + noise(rng_));
    }

    double clampGene(double v) const {
        return std::clamp(v, problem_.lower, problem_.upper);
    }

    // ---- Step 5: build the next generation ---------------------------------
    // Precondition: population_ is sorted best-first (so elites are at front).
    std::vector<Individual> nextGeneration() {
        std::vector<Individual> next;
        next.reserve(cfg_.populationSize);

        // Elitism: copy the best individuals unchanged so progress is never lost.
        for (std::size_t i = 0; i < cfg_.eliteCount; ++i)
            next.push_back(population_[i]);

        // Fill the rest with offspring.
        std::uniform_real_distribution<double> coin(0.0, 1.0);
        while (next.size() < cfg_.populationSize) {
            const Individual& p1 = tournamentSelect();
            const Individual& p2 = tournamentSelect();

            Individual child;
            // With probability crossoverRate recombine; otherwise clone parent 1.
            child.genes = (coin(rng_) < cfg_.crossoverRate)
                              ? crossover(p1.genes, p2.genes)
                              : p1.genes;
            mutate(child.genes);
            next.push_back(std::move(child));
        }
        return next;
    }

    // ---- Progress reporting -------------------------------------------------
    // "diversity" = mean distance of genes from the population's mean genome.
    // If it collapses to ~0 early, the GA has converged prematurely.
    void report(std::size_t gen) const {
        double sum = 0.0;
        for (const Individual& ind : population_) sum += ind.fitness;
        const double mean = sum / static_cast<double>(population_.size());

        std::vector<double> centroid(problem_.dimensions, 0.0);
        for (const Individual& ind : population_)
            for (std::size_t i = 0; i < centroid.size(); ++i) centroid[i] += ind.genes[i];
        for (double& c : centroid) c /= static_cast<double>(population_.size());

        double spread = 0.0;
        for (const Individual& ind : population_)
            for (std::size_t i = 0; i < centroid.size(); ++i)
                spread += std::abs(ind.genes[i] - centroid[i]);
        spread /= static_cast<double>(population_.size() * centroid.size());

        std::cout << "gen " << std::setw(4) << gen << " | best " << std::scientific
                  << std::setprecision(4) << population_.front().fitness
                  << " | mean " << mean << " | diversity " << std::fixed
                  << std::setprecision(4) << spread << '\n';
    }

    Problem problem_;
    Config cfg_;
    std::mt19937 rng_;  // Mersenne Twister: fast, high quality, seedable
    std::vector<Individual> population_;
    Individual best_;
};