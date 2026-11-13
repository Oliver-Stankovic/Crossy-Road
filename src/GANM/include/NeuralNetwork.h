#pragma once    

#include <vector>
#include <random>
#include <functional>

#include "configs.h"
#include "activationfuncs.h"

using namespace std;

// neuron ima jednu tezinu za svaki ulaz i jedan bias
struct Neuron {
    vector<double> weights;
    double bias = 0.0;
    function<double(double)> activation;

    // izlaz neurona
    double calculate(const vector<double>& inputs) const {
      
        // racunamo sumu
        double sum = bias;
        for (size_t i = 0; i < inputs.size(); ++i) {
            sum += inputs[i] * weights[i];
        }

        // Aktivacijska funkcija
        if (activation){
            sum = activation(sum);
        }
        return sum;
    }
};

class NeuralNetwork {
public:
    // neurone slazemo u slojeve (skriveni i izlazni)
    NeuralNetwork(const NN_Config config){
        //inputCount_ = config.architecture.front(); 
        rng_.seed(config.seed);
        uniform_real_distribution<double> initialvalue(config.lowerinit, config.upperinit);
        for (size_t layer = 1; layer < config.architecture.size(); ++layer) {
            vector<Neuron> neurons;
            for (size_t n = 0; n < config.architecture[layer]; ++n) {
                Neuron neuron;
                for (size_t i = 0; i < config.architecture[layer - 1]; ++i) {
                    neuron.weights.push_back(initialvalue(rng_));
                }
                neuron.bias = initialvalue(rng_); //pocetni bias nije 0.0
                if (layer != (config.architecture.size()-1)){
                    neuron.activation = ReLU;
                }
                neurons.push_back(neuron);
            }
            layers_.push_back(neurons);
        }
}

    // prolaz unaprijed: izlazi jednog sloja postaju ulazi sljedeceg
    vector<double> forward(const vector<double> inputs) const {
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
    vector<vector<Neuron>> getLayers() const {
        return layers_;
    }

    void setLayers(vector<vector<Neuron>> inplayers){
        layers_ = inplayers;
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

    void outputParameters(){
        cout << "----------------------\n";
        for (auto& layer : layers_) {
            cout << "\n";
            for (Neuron& neuron : layer) {
                for (double& weight : neuron.weights) {
                    cout << weight << ", ";
                }
                cout << "Bias: " << neuron.bias << "\n";
            }
        }
        cout << "---------------------\n";
    }

private:
    //size_t inputCount_;
    vector<vector<Neuron>> layers_;
    mt19937 rng_;
};
