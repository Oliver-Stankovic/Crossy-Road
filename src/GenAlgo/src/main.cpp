#include "GeneticAlgorithm.h"
#include "structs.h"
#include "problems.h"

#include <iostream>
#include <cstdint>
#include <string>

int main(int argc, char** argv) {
    try {
        const std::string problemName = (argc > 1) ? argv[1] : "rastrigin";
        Config cfg;
        if (argc > 2) cfg.seed = static_cast<std::uint32_t>(std::stoul(argv[2]));

        GeneticAlgorithm ga(byName(problemName), cfg);

        std::cout << "Problem: " << ga.problem().name << " (" << ga.problem().dimensions
                  << " dims, range [" << ga.problem().lower << ", "
                  << ga.problem().upper << "]), seed " << cfg.seed << "\n\n";

        const Individual best = ga.run();

        std::cout << "\nBest fitness: " << std::scientific << std::setprecision(6)
                  << best.fitness << "  (global optimum is 0)\n"
                  << "Best genes:   " << std::fixed << std::setprecision(4);
        for (double g : best.genes) std::cout << g << ' ';
        std::cout << '\n';
    } catch (const std::exception& e) {
        std::cerr << "error: " << e.what() << '\n';
        return 1;
    }
    return 0;
}