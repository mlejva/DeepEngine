#pragma once

#include <vector>
#include <algorithm>
#include <iostream>
#include "Matrix.h"
#include "Layers/IdentityLayer.h"
#include "Layers/ReluLayer.h"
#include "Layers/LayerInterface.h"

template <typename T>
class Graph {
/* Private Properties */
private:
    std::vector<std::unique_ptr<Layers::LayerInterface<T>>> layers_;
    Matrix<T> input_;
    Matrix<T> output_;

/* Private Methods */
private:
    void InitializeLayerStack() {
        std::unique_ptr<Layers::LayerInterface<T>> inputLayer_(new Layers::IdentityLayer<T>(input_));
        layers_.push_back(std::move(inputLayer_));
    }

    // Connects the output of the first layer to the input of the second layer
    void ConnectTwoLayers(const std::unique_ptr<Layers::LayerInterface<T>>& first, 
                          const std::unique_ptr<Layers::LayerInterface<T>>& second) {
        const auto& firstOutput_ = first->GetOutput();
        second->SetInput(firstOutput_);
    }

/* Constructors & Destructor */
public:
    Graph() : input_(0, 0) { InitializeLayerStack(); }
    Graph(const Matrix<T>& input) : input_(input) { InitializeLayerStack(); }
    ~Graph() { }
/* Public Methdos */
public:
    template <typename LayerType>
    const std::unique_ptr<Layers::LayerInterface<T>>& AddLayer() {        
        const auto& previousLayer_ = layers_.back();
        const auto& previousOutput_ = previousLayer_->GetOutput();
        std::unique_ptr<Layers::LayerInterface<T>> newLayer_(new LayerType(previousOutput_));

        layers_.push_back(std::move(newLayer_));

        return layers_.back();
    }

    void RemoveLayer(const std::unique_ptr<Layers::LayerInterface<T>>& layer) {
        const std::size_t& pos_ = std::find(layers_.begin(), layers_.end(), layer) - layers_.begin();        
        
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

    const Matrix<T>& Run() {
        std::size_t i = 0;
        for (const auto& layer : layers_) {
            
            std::cout << "Layer Type: " << typeid(*layer).name() << std::endl;
            std::cout << "Input:" << std::endl;
            std::cout << layer->GetInput() << std::endl;
            
            std::cout << "--------" << std::endl;

            std::cout << "Output:" << std::endl;
            std::cout << layer->GetOutput() << std::endl;
            std::cout << "========" << std::endl;
            
            //if (i == layers_.size() - 1) {
             //   output_ = layer->GetOutput();
            //}
            ++i;            
        }

        return output_;
    }

    const Matrix<T>& Run2() {
        output_ = layers_.back()->GetOutput();
        return output_;
    }
};
