#pragma once

#include <vector>
#include <unordered_map>
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
    Matrix<T>& input_;
    Matrix<T> output_;
    Matrix<T>& expectedOutput_;
    
    std::vector<std::unique_ptr<Layers::LayerInterface<T>>> layers_;

    std::unique_ptr<Functions::LossFunctionInterface<T>> lossFunction_;

/* Private Methods */
private:    
    void InitializeLayerStack_(const Matrix<T>& newInput) {
        // 1) Either layers_ are not yet initialized (i.e. user just created an instance of and wants to add layers)
        //  -> Push input layer back onto layers_

        // 2) Or layers_ are already initialized and user wants either train or evaluate
        //  -> Change the input of an input layer    

        if (layers_.size() == 0) {
            // Here number of layers' outputs is 'input_.GetRowsCount()' because we are using this layer as a "dummy" layer
            // ---> 'layers_' can't be empty when user is calling 'AddLayer()'. That's why we create this dummy layer 
            std::unique_ptr<Layers::LayerInterface<T>> inputLayer_(new Layers::InputLayer<T>(newInput));
            layers_.push_back(std::move(inputLayer_));
        }
        else {
            layers_.front()->Initialize(newInput);
        }
    }

    void Forward_() {
        for (const auto& layer : layers_) { layer->Forward(); }
    }

    void Backward_() {                   
        const auto& lastLayer_ = layers_.back();        
        
        // Vector of vectors
        std::vector<std::vector<Matrix<T>>> layerErrorsForAllInputs_(input_.GetRowsCount());

        // First axis of an input is the batch size
        // for each single input vector...
        for (auto i = 0; i < input_.GetRowsCount(); ++i) {
            const auto& singlePredictedOutput_ = lastLayer_->GetOutput().GetRow(i);
            const auto& singleExpectedOutput_ = expectedOutput_.GetRow(i);            

            const auto& lossGradient_ = lossFunction_->ComputeGradient(singlePredictedOutput_, singleExpectedOutput_);
            const auto& outputErrorForSingleInput_ = lastLayer_->ComputeLayerError(lossGradient_, i);            

            // Computes an error for each layer for the given input
            const auto& layerErrorsForSingleInput_ = BackpropagateError_(outputErrorForSingleInput_, i);
            layerErrorsForAllInputs_[i] = layerErrorsForSingleInput_;            
        }        

        // Stochastic Gradient Descent -> udpate weights
        SGD_(layerErrorsForAllInputs_, 0.001);             
    }     

    const std::vector<Matrix<T>> BackpropagateError_(const Matrix<T>& outputError, const std::size_t inputIndex) {
        const auto& layerErrorsSize_ = layers_.size() - 1; // -1 because the input layer is not hidden nor output layer (=> no weights)
        std::vector<Matrix<T>> layerErrors_(layerErrorsSize_);
        layerErrors_.[layerErrorsSize_ - 1] = outputError;      

        // - 2 because error for the output layer is already computed
        const auto& layersTotal_ = layers_.size();
        for (auto layerIndex = layersTotal_ - 2; layerIndex > 0; --layerIndex) {            
            const auto& followingLayer_ = layers_.]layerIndex + 1];
            const auto& currentLayer_ = layers_.[layerIndex];

            const auto& followingWeights_ = followingLayer_->GetWeights();            
            
            const auto& followingLayerError_ = layerErrors_.[layerIndex];            
            const auto& movedError_ = followingWeights_ * followingLayerError_;

            const auto& currentLayerError_ = currentLayer_->ComputeLayerError(movedError_, inputIndex);
            
            layerErrors_[layerIndex - 1] = currentLayerError_; // -1 becuase layerIndex is shifted by 1 against the layerErrorsSize_
        }

        return layerErrors_;
    } 

    // TODO: What datatype for learningRate?
    void SGD_(const std::vector<std::vector<Matrix<T>>>& layerErrors_, const double& learningRate) {
        const auto& batchSize_ = input_.GetRowsCount();
        const auto& layersTotal_ = layers_.size();

        for (auto layerIndex = layersTotal_ - 1; layerIndex > 0; --layerIndex) {
            const auto& currentLayer_ = layers_.at(layerIndex);
            const auto& previousLayer_ = layers_.at(layerIndex - 1);

            Matrix<T> layerDeltaWeights_;
            // Sum layer errors for the given single input
            for (auto inputIndex = 0; inputIndex < input_.GetRowsCount(); ++inputIndex) {                
                // Returns a vector of Matrix<T> 
                // length of this returned error is same as layersTotal_ - 1 (input layer has no errors/weights)
                const auto& layerErrorsForSingleInput_ = layerErrors_[inputIndex]; 

                // Returns a Matrix<T>
                // this is an error for the specific layer given by the layerIndex
                const auto& error_ = layerErrorsForSingleInput_[layerIndex - 1];
                const auto& activations_ = previousLayer_->GetOutput().GetRow(inputIndex);

                auto& deltaWeights_ = activations_.Transpose() * error_.Transpose();                
                if (layerDeltaWeights_ != deltaWeights_)
                    layerDeltaWeights_.InitializeWithZeros(deltaWeights_.GetRowsCount(), deltaWeights_.GetColsCount());
                
                layerDeltaWeights_ += deltaWeights_;
            }

            layerDeltaWeights_ = layerDeltaWeights_ * (learningRate / batchSize_);
            currentLayer_->UpdateWeights(layerDeltaWeights_);
        }
    }   

/* Constructors & Destructor */
public:
    Graph() : input_(0, 0), expectedOutput_(0, 0) { 
        lossFunction_ = std::unique_ptr<Functions::LossFunctionInterface<T>>(new LossFunctionType()); 
        InitializeLayerStack_(input_);
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

    const Matrix<T>& Run() { }

    const Matrix<T>& Train(const Matrix<T>& input, const Matrix<T>& expectedOutput) {
/*        expectedOutput_ = expectedOutput
        batchSize_ = input.GetRows();
        InitializeLayers_(input);


        input_ = input;
        expectedOutput_ = expectedOutput_;        
        InitializeLayerStack_(input_); 
*/
        // Compute activations
        Forward_();        

        output_.ReshapeWithMatrix(layers_.back()->GetOutput());
        const auto& loss_ = lossFunction_->Apply(output_, expectedOutput_);
        //const auto& derivative_ = lossFunction_->ComputeDerivative(output_, expectedOutput_);

        std::cout << "Loss: " << loss_ << std::endl;
        //std::cout << "Loss derivative: " << derivative_ << std::endl;

        // Compute error of each layer and update weights
        Backward_();        

        return output_;
    }

    void Evaluate() {
        // Only forward
    }
};
