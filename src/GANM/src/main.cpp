#include <iostream>
#include "NeuralNetwork.h"
#include "GeneticAlgorithm.h"
#include "../../model/Input.hpp"
#include "../../model/Model.hpp"
#include <random>
using namespace std;

vector<double> pretvoriZrake(const vector<float>& zrake) {
    vector<double> ulazi;
    for(float zraka : zrake) {
        ulazi.push_back(static_cast<double>(zraka));
    }
    return ulazi;
}

Input odaberiPotez(const vector<double>& izlazi) {
    const Input potezi[] = {
        Input::None, Input::Left, Input::Right, Input::Up, Input::Down
    };

    size_t najboljiIndeks = 0;
    for(size_t i = 1; i < izlazi.size(); ++i) {
        if (izlazi[i] > izlazi[najboljiIndeks]) najboljiIndeks = i;
    }
    return potezi[najboljiIndeks];
}

int main() {

    NN_Config postavkeIgre;
    postavkeIgre.architecture = {32, 6, 5};

    NeuralNetwork mrezaZaIgru(postavkeIgre);
    CrossyRoadModel model;
    model.Start();

    const int maxKoraka = 1000;
    int korak = 0;

    while (model.isPlayerAlive() && korak < maxKoraka) {
        vector<float> zrake = model.castRays(32);
        vector<double> ulazi = pretvoriZrake(zrake);
        vector<double> ocjenePoteza = mrezaZaIgru.forward(ulazi);
        Input potez = odaberiPotez(ocjenePoteza);

        model.Update(0.016f, potez);
        ++korak;

        if (korak <= 10) {
            const char* nazivPoteza = "nepoznato";

            switch (potez)
            {
            case Input::None: nazivPoteza = "cekaj"; break;
            case Input::Left: nazivPoteza = "lijevo"; break;
            case Input::Right: nazivPoteza = "desno"; break;
            case Input::Up: nazivPoteza = "gore"; break;
            case Input::Down: nazivPoteza = "dolje"; break;
            }
            
            cout << "korak " << korak << ": mreza je odabrala " << nazivPoteza << '\n';
            
        }
    }

    cout << "odigrano koraka: " << korak << '\n';
    cout << "igrac je ziv:" << (model.isPlayerAlive() ? "da" : "ne") << '\n';
    cout << "rezultat: " << model.getPlayerScore() << '\n';

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