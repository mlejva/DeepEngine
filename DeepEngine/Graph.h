#pragma once

#include <stack>
#include "Matrix.h"
#include "ActivationFunctionInterface.h"
#include "ReluActivationFunction.h"
#include "SigmoidActivationFunction.h"
#include "TanhActivationFunction.h"

template <typename T>
class Graph {

private:
    class LayerInterface {
    friend class Graph;
    protected:
        Matrix<T> bias_;
        Matrix<T> weights_;
        

    public:
        virtual void InitializeWeights() = 0;
        virtual void SetBiasMatrix(Matrix<T> biasMatrix) = 0;
    };

public:
    /*class FullyConnectedLayer : public LayerInterface {

    };

    class ReluLayer : public LayerInterface {

    };    

    class SigmoidLayer : public LayerInterface {

    };    

    class TanhLayer : public LayerInterface {
        
    };*/

private:
    std::stack<LayerInterface> layers_;
    Matrix<T> input_;

public:
    Graph() { std::cout << input_ << std::endl; }
    Graph(Matrix<T> input) : input_(input) { }

    void SetInput(Matrix<T> input) {
        input_ = input;
    }

    void AddLayer(LayerInterface l) {
        // TODO: Access the element on the top of layers and connect newly added layer to the layer on the top
        layers_.push(l);
    }

    void RemoveLayer() {
        layers_.pop();
    }

    void Train() {

    }
    
    void Test() {

    }
};
