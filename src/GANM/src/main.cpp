// NOVO
#include <iostream>
#include "NeuralNetwork.h"
#include "GeneticAlgorithm.h"
#include <random>
using namespace std;

// double izracunajPogresku(const NeuralNetwork& network,
//                         const vector<vector<double>>& input,
//                         const vector<double>& target) {
//     double ukupnaPogreska = 0.0;

//     for (size_t i = 0; i < input.size(); ++i) {
//         const auto output = network.forward(input[i]);
//         double razlika = target[i] - output[0];
//         ukupnaPogreska += razlika * razlika;
//         // kvadriramo razliku da se poz i neg pogreske ne poniste pri zbrajanju
//         // i da veci promasaji vise utjecu na ukupnu pogresku
//     }

//     return ukupnaPogreska / input.size(); // vraca prosjek
//}

int main() {
    GA_Config gaconfig;
    GeneticAlgorithm algoritam(gaconfig);

    Candidate najbolji = algoritam.run();

    cout << "broj kandidata: " << gaconfig.populationSize << '\n';
    cout << "najbolja pocetna pogreska: " << najbolji.fitness << '\n';

    // NOVO - provjera najbolje mreze na sva cetiri AND primjera
    for (size_t i = 0; i < gaconfig.input.size(); ++i) {
        const auto& ulaz = gaconfig.input[i];
        double izlaz = najbolji.neuralnetwork.forward(ulaz)[0];
        int odluka = (izlaz >= 0.5) ? 1 : 0;

        cout << "ulaz: [" << ulaz[0] << ", " << ulaz[1] << "]"
         << ", izlaz mreze: " << izlaz
         << ", odluka: " << odluka
         << ", zeljeno: [";
        for (size_t j = 0; j < gaconfig.target[i].size(); ++j) {
            if (j > 0) cout << ", ";
            cout << gaconfig.target[i][j];
        }
        cout << "]\n";
    }
}