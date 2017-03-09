#pragma once

#include <stack>
#include "Matrix.h"
#include "Functions/ActivationFunctions/ActivationFunctionInterface.h"
#include "Functions/ActivationFunctions/ReluActivationFunction.h"
#include "Functions/ActivationFunctions/SigmoidActivationFunction.h"
#include "Functions/ActivationFunctions/TanhActivationFunction.h"
#include "Layers/LayerInterface.h"

template <typename T>
class Graph {

private:
    std::stack<LayerInterface<T>> layers_;
    Matrix<T> input_;

public:
    Graph() { std::cout << input_ << std::endl; }
    Graph(Matrix<T> input) : input_(input) { }

    void SetInput(Matrix<T> input) {
        input_ = input;
    }

    template <typename LayerType>
    LayerInterface<T>& AddLayer() {
        // TODO: Access the element on the top of layers and connect newly added layer to the layer on the top        
    }

    void RemoveLayer() {
        layers_.pop();
    }
};
