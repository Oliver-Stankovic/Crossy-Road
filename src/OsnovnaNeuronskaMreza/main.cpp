#include "NeuralNetwork.h"

#include <iomanip>
#include <iostream>

using namespace std;

int main() {
    
    try {
        NeuralNetwork network({2, 2, 1});   // 2 (ulazna podatka) -> 2 (skrivena neurona) -> 1 (izlazni neuron)

        // rucno odabrani parametri - 2 tezine i bias
        network.setParameters({
            1.0, 0.0, 0.0,  // prvi skriveni neuron: w1, w2, bias
            0.0, 1.0, 0.0,  // drugi skriveni neuron: w1, w2, bias
            1.0, 1.0, -1.0  // izlazni neuron: w1, w2, bias
        });

        // zbroj = prvi ulaz x 1 + drugi ulaz * 0 + 0

        cout << "mreza: 2 -> 2 -> 1\n";
        cout << "broj parametara: " << network.parameterCount() << "\n\n";
        cout << fixed << setprecision(6);

        // ulazi

        const vector<vector<double>> examples = {
            {0.0, 0.0}, {1.0, 0.0}, {0.0, 1.0}, {1.0, 1.0}
        };

        for (const auto& inputs : examples) {
            const auto outputs = network.forward(inputs);
            cout << "ulaz: [" << inputs[0] << ", " << inputs[1] << "] -> izlaz: " << outputs[0] << '\n';
        }

    } catch (const exception& error) {
        cerr << "greska: " << error.what() << '\n';
        return 1;
    }
}
