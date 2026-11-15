#include <iostream>
#include <random>

#include "NeuralNetwork.h"
#include "GeneticAlgorithm.h"
#include "../../model/Model.hpp"

using namespace std;

int main() {

    NN_Config postavkeIgre;
    postavkeIgre.architecture = {32, 6, 5};

    NeuralNetwork mrezaZaIgru(postavkeIgre);
    CrossyRoadModel model;

    GA_Config gaconfig;
    GeneticAlgorithm algoritam(gaconfig);

    const int maxKoraka = 1000;

    Candidate najbolji = algoritam.run(model, maxKoraka);

    cout << "Number of candidates: " << gaconfig.populationSize << '\n';
    cout << "Best candidate fitness: " << najbolji.fitness << '\n';

    // int korak = 0;

    // while (model.isPlayerAlive() && korak < maxKoraka) {
    //     vector<float> zrake = model.castRays(32);
    //     vector<double> ulazi = pretvoriZrake(zrake);
    //     vector<double> ocjenePoteza = mrezaZaIgru.forward(ulazi);
    //     Input potez = odaberiPotez(ocjenePoteza);

    //     model.Update(0.016f, potez);
    //     ++korak;

    //     if (korak <= 10) {
    //         const char* nazivPoteza = "nepoznato";

    //         switch (potez)
    //         {
    //         case Input::None: nazivPoteza = "cekaj"; break;
    //         case Input::Left: nazivPoteza = "lijevo"; break;
    //         case Input::Right: nazivPoteza = "desno"; break;
    //         case Input::Up: nazivPoteza = "gore"; break;
    //         case Input::Down: nazivPoteza = "dolje"; break;
    //         }
            
    //         cout << "korak " << korak << ": mreza je odabrala " << nazivPoteza << '\n';
            
    //     }
    // }

    // cout << "odigrano koraka: " << korak << '\n';
    // cout << "igrac je ziv:" << (model.isPlayerAlive() ? "da" : "ne") << '\n';
    // cout << "rezultat: " << model.getPlayerScore() << '\n';




    // for (size_t i = 0; i < gaconfig.input.size(); ++i) {
    //     const auto& ulaz = gaconfig.input[i];
    //     vector<double> izlaz = najbolji.neuralnetwork.forward(ulaz);
        

    //     cout << "ulaz: [" ;
    //     for (size_t j = 0; j < ulaz.size(); ++j) {
    //         if (j > 0) cout << ", ";
    //         cout << ulaz[j];
    //     }
    //     cout << "], izlaz mreze: [";
    //     for (size_t j = 0; j < izlaz.size(); ++j) {
    //         if (j > 0) cout << ", ";
    //         cout << izlaz[j];
    //     }
    //     cout << "], zeljeno: [";
    //     for (size_t j = 0; j < gaconfig.target[i].size(); ++j) {
    //         if (j > 0) cout << ", ";
    //         cout << gaconfig.target[i][j];
    //     }
    //     cout << "]\n";
    // } 

    //najbolji.neuralnetwork.outputParameters();

}