// NOVO
#include <iostream>
#include "NeuralNetwork.h"
#include "GeneticAlgorithm.h"
#include <random>
using namespace std;

int main() {
    GA_Config gaconfig;
    GeneticAlgorithm algoritam(gaconfig);

    Candidate najbolji = algoritam.run();

    cout << "Number of candidates: " << gaconfig.populationSize << '\n';
    cout << "Best candidate fitness: " << najbolji.fitness << '\n';


    // NOVO - provjera najbolje mreze na sva cetiri AND primjera
    for (size_t i = 0; i < gaconfig.input.size(); ++i) {
        const auto& ulaz = gaconfig.input[i];
        vector<double> izlaz = najbolji.neuralnetwork.forward(ulaz);
        

        cout << "ulaz: [" ;
        for (size_t j = 0; j < ulaz.size(); ++j) {
            if (j > 0) cout << ", ";
            cout << ulaz[j];
        }
        cout << "], izlaz mreze: [";
        for (size_t j = 0; j < izlaz.size(); ++j) {
            if (j > 0) cout << ", ";
            cout << izlaz[j];
        }
        cout << "], zeljeno: [";
        for (size_t j = 0; j < gaconfig.target[i].size(); ++j) {
            if (j > 0) cout << ", ";
            cout << gaconfig.target[i][j];
        }
        cout << "]\n";
    }

    //najbolji.neuralnetwork.outputParameters();

}