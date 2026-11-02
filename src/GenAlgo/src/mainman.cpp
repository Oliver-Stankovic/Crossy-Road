#include <iostream>
#include <string>
#include <vector>

#include "funcs.h"

using namespace std;

int main(int argc, char** argv){

    string sequence;
    cout << "Enter the target sequence: ";
    cin >> sequence;

    if (!is_valid_sequence(sequence)){
        cout << "Invalid sequence";
        return 0;
    }

    size_t popcount;
    cout << "Enter the population count: ";
    cin >> popcount;

    vector<Candidate> population = generate_population(popcount);

    size_t numgenerations;
    cout << "Enter the enter the number of generations: ";
    cin >> numgenerations;

    for (size_t i = 0; i<numgenerations; ++i){
        //svaki kandidat u populaciji je ocijenjen
        for (Candidate& candidate : population){ 
            score(candidate, sequence);
        }

        //populacija sortirana po ocjeni
        std::sort(population.begin(), population.end(), [](const Candidate& a, const Candidate& b) {
            return a.fitness > b.fitness; 
        });

        //zadrzi n najboljih kandidata
        //stvori novu djecu sa crossover i mutacijom
        population = newGeneration(3, population);

        if (i%5 == 0){
            cout << "Najbolji za generaciju " << i << " je " << population.front().value << "||"; 
            cout << "Ocjena: " << population.front().fitness << "\n";
        }
    }

    cout << "Najbolji na kraju je " << population.front().value << " || ";
    cout << "Za ciljanu sekvencu " << sequence;
}