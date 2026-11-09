#pragma once    

#include <cmath>
#include <cstddef>
#include <stdexcept>
#include <vector>

using namespace std;

// neuron ima jednu tezinu za svaki ulaz i jedan bias
struct Neuron {
    vector<double> weights;
    double bias = 0.0;

    // izlaz neurona
    double calculate(const vector<double>& inputs) const {
      
        // racunamo sumu
        double sum = bias;
        for (size_t i = 0; i < inputs.size(); ++i) {
            sum += inputs[i] * weights[i];
        }

        // sigmoida -> broj pretvori u vrijednost izmedu 0 i 1
        return 1.0 / (1.0 + exp(-sum));
    }
};

class NeuralNetwork {
public:
    
    // neurone slazemo u slojeve (skriveni i izlazni)
    NeuralNetwork(const vector<size_t>& architecture) {
    inputCount_ = architecture.front(); 
    for (size_t layer = 1; layer < architecture.size(); ++layer) {
        vector<Neuron> neurons;
        for (size_t n = 0; n < architecture[layer]; ++n) {
            Neuron neuron;
            for (size_t i = 0; i < architecture[layer - 1]; ++i) {
                neuron.weights.push_back(0.0);
            }
            neurons.push_back(neuron);
        }
        layers_.push_back(neurons);
    }
}

    // prolaz unaprijed: izlazi jednog sloja postaju ulazi sljedeceg
    vector<double> forward(const vector<double>& inputs) const {
        vector<double> values = inputs;
        for (const auto& layer : layers_) {
            vector<double> outputs;
            for (const Neuron& neuron : layer) {
                outputs.push_back(neuron.calculate(values));
            }
            values = outputs;
        }
        return values;
    }

    // ukupan broj parametara
    size_t parameterCount() const {
        size_t count = 0;
        for (const auto& layer : layers_) {
            for (const Neuron& neuron : layer) {
                count += neuron.weights.size() + 1;
            }
        }
        return count;
    }

    // lista svih tezina i biasa
    vector<double> getParameters() const {
        vector<double> parameters;
        for (const auto& layer : layers_) {
            for (const Neuron& neuron : layer) {
                for (double weight : neuron.weights) {
                    parameters.push_back(weight);
                }
                parameters.push_back(neuron.bias);
            }
        }
        return parameters;
    }

    // uzima listu brojeva i postavlja ih kao tezine i biase
    void setParameters(const vector<double>& parameters) {
        size_t index = 0;
        for (auto& layer : layers_) {
            for (Neuron& neuron : layer) {
                for (double& weight : neuron.weights) {
                    weight = parameters[index++];
                }
                neuron.bias = parameters[index++];
            }
        }
    }

private:
    size_t inputCount_;
    vector<vector<Neuron>> layers_;
};
