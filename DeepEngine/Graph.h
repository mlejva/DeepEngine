#pragma once

#include <vector>
#include <array>
#include <algorithm>
#include <iostream>
#include "Matrix.h"
#include "Layers/InputLayer.h"
#include "Layers/ReluLayer.h"
#include "Layers/LayerInterface.h"
#include "Functions/LossFunctions/LossFunctionInterface.h"

template <typename T, typename LossFunctionType>
class Graph {
/* Private Properties */
private:
    Matrix<T> input_;
    Matrix<T> output_;
    Matrix<T> expectedOutput_;
    
    std::vector<std::unique_ptr<Layers::LayerInterface<T>>> layers_;

    std::unique_ptr<Functions::LossFunctionInterface<T>> lossFunction_;

/* Private Methods */
private:
    void InitializeLayerStack_() {
        // Here number of layers' outputs is 'input_.GetRowsCount()' because we are using this layer as a "dummy" layer
        // ---> 'layers_' can't be empty when user is calling 'AddLayer()'. That's why we create this dummy layer 
        std::unique_ptr<Layers::LayerInterface<T>> inputLayer_(new Layers::InputLayer<T>(input_));
        layers_.push_back(std::move(inputLayer_));
    }

    void Forward_() {
        for (const auto& layer : layers_) {
            layer->Forward();
                                    
            std::cout << "Layer Type: " << typeid(*layer).name() << std::endl;
            std::cout << "Input: " << layer->GetLayerInputShape() << std::endl;
            std::cout << layer->GetInput() << std::endl;

            std::cout << "--------" << std::endl;

            std::cout << "Output: " << layer->GetLayerOutputShape() << std::endl;
            std::cout << layer->GetOutput() << std::endl;
            std::cout << "========" << std::endl;
        }
    }

    void Backward_() {        
        // 1) Get the output error:
        // i.e. error of the last layer
        // layer_errors is a dictionary of int:Matrix<T>
        // layer_errors[last_layer_index] = dLoss/dLast_Layer_Output * last_layer_act_func_derivative(output_before_activationFunction)
        // dLoss/dLast_Layer_Output is a vector (= loss gradient with respect to the activations of last layer)
        // multiplication is the hadamard product
        
        const auto& lastLayer_ = layers_.back();
        // Loss gradient with respect to the network predicted values
        const auto& lossGradient_ = lossFunction_->ComputeGradient(lastLayer_->GetOutput(), expectedOutput_);
        const Matrix<T>& outputError_ = lastLayer_->ComputeLayerError(lossGradient_);
        
        const std::vector<Matrix<T>> layerErrors_ = BackpropagateError_(outputError_);

        // Gradient Descent

        /*
        // Returns the gradient matrix of loss with respect to the output of the last layer (= predicted)
        auto& lossGradient_ = lossFunction_–>ComputeGradient(lastLayer_->GetOutput(), expectedOutput_);
        // Returns the matrix of derivation_activ_function(output_before_activationFunction_j) for each j in that layer
        auto& activDer_ = lastLayer_->ComputeDerivative();
        auto& lastLayerError_ = lossGradient_.HadamardProduct(activDer_);
        layerErrors_[layersTotal_ - 1] = lastLayerError_;        
        */
        // 2) Backpropagate the error:
        // for i=layers_total - 2 to 0: // -2 because an error of the last layer is already computed
        //    // multiplication is the hadamard product
        //    layer_error = weights_of_layer_i+1 * layer_errors[i+1] * current_layer_act_func_derivative(current_layer_z_values) 
        //    layer_errors[i] = layer_error

        // 3) Update weights and biases
    } 
    
    const std::vector<Matrix<T>> BackpropagateError_(const Matrix<T>& outputError) {
        const auto& layersTotal_ = layers_.size();
        std::vector<Matrix<T>> layerErrors_(layersTotal_);
        layerErrors_.at(layersTotal_ - 1) = outputError;        

        // - 2 because output layer is already computed
        // on the 0 index is a dummy input layer
        for (auto layerIndex = layersTotal_ - 2; layerIndex > 0; layerIndex--) {
            std::cout << "layer: " << std::to_string(layerIndex) << std::endl;
            const auto& previousLayer_ = layers_.at(layerIndex + 1);
            const auto& currentLayer_ = layers_.at(layerIndex);

            const auto& previousWeights_ = previousLayer_->GetWeights();            
            std::cout << "previous weights of layer: " << std::to_string(layerIndex + 1) << std::endl;
            std::cout << previousWeights_ << std::endl;
            std::cout << "----end----" << std::endl;

            const auto& previousLayerError_ = layerErrors_.at(layerIndex + 1);
            std::cout << "previous error of layer: " << std::to_string(layerIndex + 1) << std::endl;
            std::cout << previousLayerError_ << std::endl;
            std::cout << "----end----" << std::endl;
            const auto& movedError_ = previousLayerError_ * previousWeights_;

            const auto& currentLayerError_ = currentLayer_->ComputeLayerError(movedError_);
            std::cout << "error of current layer: " << std::to_string(layerIndex) << std::endl;
            std::cout << currentLayerError_ << std::endl;
            std::cout << "----end----" << std::endl;
            layerErrors_[layerIndex] = currentLayerError_;
        }

        return layerErrors_;
    }

    /*
    // Connects the output of the first layer to the input of the second layer
    void ConnectTwoLayers(const std::unique_ptr<Layers::LayerInterface<T>>& first, 
                          const std::unique_ptr<Layers::LayerInterface<T>>& second) {
        const auto& firstOutput_ = first->GetOutput();
        second->SetInput(firstOutput_);
    }
    */

/* Constructors & Destructor */
public:
    Graph() : input_(0, 0), expectedOutput_(0, 0) { InitializeLayerStack_(); }
    Graph(const Matrix<T>& input, const Matrix<T>& expectedOutput) : input_(input), expectedOutput_(expectedOutput) { 
        lossFunction_ = std::unique_ptr<Functions::LossFunctionInterface<T>>(new LossFunctionType());        
        InitializeLayerStack_(); 
    }
    ~Graph() { }

/* Public Methdos */
public:
    template <typename LayerType>
    const std::unique_ptr<Layers::LayerInterface<T>>& AddLayer(const std::size_t& outputSize) {        
        const auto& previousLayer_ = layers_.back();
        const auto& previousOutput_ = previousLayer_->GetOutput();
        
        std::unique_ptr<Layers::LayerInterface<T>> newLayer_(new LayerType(previousOutput_, outputSize));        

        layers_.push_back(std::move(newLayer_));

        return layers_.back();
    }

    /*
    void RemoveLayer(const std::unique_ptr<Layers::LayerInterface<T>>& layer) {
        const auto& pos_ = std::find(layers_.begin(), layers_.end(), layer) - layers_.begin();        
        
        if (pos_ == 1 && layers_.size() > 2) { 
            // User wants to delete the first layer after the input layer 
            // And vector of layers has more than two layers
            // (user never has a reference to the input layer)

            // Can we connect the input layer to a next layer?
            if (layers_.size() - 1 <= pos_ + 1) { 
                const auto& inputLayer_ = layers_.at(0);
                const auto& nextLayer_ = layers_.at(pos_ + 1);
                ConnectTwoLayers(inputLayer_, nextLayer_);                
            }
        }
        else if (pos_ == layers_.size() - 1) { 
            // User wants to delete the last layer
            // No need for any reconnecting

            // ...
        }
        else {
            // User wants to delete a layer in the middle 
            // Both previus and next layer is defined

            const auto& previousLayer_ = layers_.at(pos_ - 1);
            const auto& nextLayer_ = layers_.at(pos_ + 1);
            ConnectTwoLayers(previousLayer_, nextLayer_);
        }

        layers_.erase(layers_.begin() + pos_); // Delete the layer
    }
    */

    const Matrix<T>& Run() {

        // TRAIN:
        // Compute activations
        Forward_();
        // Compute error of each layer
        Backward_();
        // TODO: Update weights
        //SGD_();

        output_.ReshapeWithMatrix(layers_.back()->GetOutput());
        const auto& loss_ = lossFunction_->Apply(output_, expectedOutput_);
        //const auto& derivative_ = lossFunction_->ComputeDerivative(output_, expectedOutput_);

        std::cout << "Loss: " << loss_ << std::endl;
        //std::cout << "Loss derivative: " << derivative_ << std::endl;
        return output_;
    }

    void Train() {

    }

    void Evaluate() {
        // Only forward
    }
};
