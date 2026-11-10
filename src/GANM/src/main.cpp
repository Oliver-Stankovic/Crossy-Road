// NOVO
#include <iostream>
#include "NeuralNetwork.h"
#include "GeneticAlgorithm.h"
#include <random>
using namespace std;

double izracunajPogresku(const NeuralNetwork& network,
                        const vector<vector<double>>& examples,
                        const vector<double>& zeljeniIzlazi) {
    double ukupnaPogreska = 0.0;

    for (size_t i = 0; i < examples.size(); ++i) {
        const auto output = network.forward(examples[i]);
        double razlika = zeljeniIzlazi[i] - output[0];
        ukupnaPogreska += razlika * razlika;
        // kvadriramo razliku da se poz i neg pogreske ne poniste pri zbrajanju
        // i da veci promasaji vise utjecu na ukupnu pogresku
    }

    return ukupnaPogreska / examples.size(); // vraca prosjek
}

int main() {
    GA_Config config;
    GeneticAlgorithm algoritam(config);

    Candidate najbolji = algoritam.run();

    cout << "broj kandidata: " << config.populationSize << '\n';
    cout << "najbolja pocetna pogreska: " << najbolji.fitness << '\n';

    // NOVO - provjera najbolje mreze na sva cetiri AND primjera
    for (size_t i = 0; i < config.examples.size(); ++i) {
        const auto& ulaz = config.examples[i];
        double izlaz = najbolji.neuralnetwork.forward(ulaz)[0];
        int odluka = (izlaz >= 0.5) ? 1 : 0;

        cout << "ulaz: [" << ulaz[0] << ", " << ulaz[1] << "]"
         << ", izlaz mreze: " << izlaz
         << ", odluka: " << odluka
         << ", zeljeno: " << config.zeljeniIzlazi[i] << '\n';
}
}