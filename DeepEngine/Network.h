#pragma once

#include <tuple>
#include <vector>
#include <unordered_map>
#include <array>
#include <algorithm>
#include <iostream>
#include "./Matrix.h"
#include "./Layers/InputLayer.h"
#include "./Layers/ReluLayer.h"
#include "./Layers/LayerInterface.h"
#include "./Functions/LossFunctions/LossFunctionInterface.h"

template <typename T, typename LossFunctionType>
class Network {
/* Private Properties */
private:    
    std::vector<std::unique_ptr<Layers::LayerInterface<T> > > layers_;
    std::unique_ptr<Functions::LossFunctionInterface<T> > lossFunction_;

    std::unique_ptr<Matrix<T> > input_;

/* Private Typedefs */
private:
    // Loss and network predictions
    typedef std::tuple<double, Matrix<T>> T_networkOutput;


/* Private methods for traversing the computational graph */
private:
    T_networkOutput RunIteration_(const Matrix<T>& input, const Matrix<T>& expectedOutput) {                
        auto targets_ = Forward_(input, expectedOutput);        

        // Compute error of each layer and update weights
        Backward_(input, expectedOutput); 

        return targets_;
    }       

    T_networkOutput Forward_(const Matrix<T>& input, const Matrix<T>& expectedOutput) {
        ComputeActivations_(input);        

        const auto& predictedOutput_ = layers_.back()->GetOutput();
        const auto& loss_ = lossFunction_->Apply(predictedOutput_, expectedOutput);

        return std::make_tuple(loss_, predictedOutput_);
    }    

    // Computes error of each layer and updates weights
    void Backward_(const Matrix<T>& input, const Matrix<T>& expectedOutput) {                   
        const auto& lastLayer_ = layers_.back();        
        
        // Vector of vectors
        // Each inner vector contains errors of all layers for the single given input
        std::vector<std::vector<Matrix<T>>> layerErrorsForAllInputs_(input.GetRowsCount());

        // First axis of an input is the batch size
        // for each single input vector...
        for (std::size_t i = 0; i < input.GetRowsCount(); ++i) {
            const auto& singlePredictedOutput_ = lastLayer_->GetOutput().GetRow(i);
            const auto& singleExpectedOutput_ = expectedOutput.GetRow(i);            

            const auto& lossGradient_ = lossFunction_->ComputeGradient(singlePredictedOutput_, singleExpectedOutput_);
            const auto& outputErrorForSingleInput_ = lastLayer_->ComputeLayerError(lossGradient_, i);            

            // Computes an error for each layer for the given input
            const auto& layerErrorsForSingleInput_ = BackpropagateError_(outputErrorForSingleInput_, i);
            layerErrorsForAllInputs_[i] = layerErrorsForSingleInput_;            
        }        

        // Stochastic Gradient Descent -> udpate weights
        const auto& batchSize_ = input.GetRowsCount();
        SGD_(batchSize_, layerErrorsForAllInputs_, 0.01);             
    }     

     void ComputeActivations_(const Matrix<T>& input) {
        // Inserts new input into input layer
        layers_.front()->Initialize(input);
        
        for (const auto& layer : layers_) { layer->Forward(); }
    }

    const std::vector<Matrix<T>> BackpropagateError_(const Matrix<T>& outputError, const std::size_t inputIndex) {
        const auto& layerErrorsSize_ = layers_.size() - 1; // -1 because the input layer is not hidden nor output layer (=> no weights)
        std::vector<Matrix<T>> layerErrors_(layerErrorsSize_);
        layerErrors_[layerErrorsSize_ - 1] = outputError;      

        // - 2 because error for the output layer is already computed
        const auto& layersTotal_ = layers_.size();
        for (std::size_t layerIndex = layersTotal_ - 2; layerIndex > 0; --layerIndex) {            
            const auto& followingLayer_ = layers_[layerIndex + 1];
            const auto& currentLayer_ = layers_[layerIndex];

            const auto& followingWeights_ = followingLayer_->GetWeights();            
            
            const auto& followingLayerError_ = layerErrors_[layerIndex];            
            const auto& movedError_ = followingWeights_ * followingLayerError_;

            const auto& currentLayerError_ = currentLayer_->ComputeLayerError(movedError_, inputIndex);
            
            layerErrors_[layerIndex - 1] = currentLayerError_; // -1 becuase layerIndex is shifted by 1 against the layerErrorsSize_
        }

        return layerErrors_;
    } 

    // TODO: What data type for learningRate?
    void SGD_(const std::size_t& batchSize, const std::vector<std::vector<Matrix<T>>>& layerErrors_, const double& learningRate) {
        const auto& layersTotal_ = layers_.size();

        for (std::size_t layerIndex = layersTotal_ - 1; layerIndex > 0; --layerIndex) {
            const auto& currentLayer_ = layers_.at(layerIndex);
            const auto& previousLayer_ = layers_.at(layerIndex - 1);

            Matrix<T> layerDeltaWeights_;
            Matrix<T> layerDeltaBias_;
            // Sum layer errors for the given single input
            for (std::size_t inputIndex = 0; inputIndex < batchSize; ++inputIndex) {                
                // Returns a vector of Matrix<T> 
                // length of this returned error is same as layersTotal_ - 1 (input layer has no errors/weights)
                const auto layerErrorsForSingleInput_ = layerErrors_[inputIndex]; 

                // Returns a Matrix<T>
                // this is an error for the specific layer given by the layerIndex
                const auto& error_ = layerErrorsForSingleInput_[layerIndex - 1];
                const auto& activations_ = previousLayer_->GetOutput().GetRow(inputIndex);

                auto& deltaWeights_ = activations_.Transpose() * error_.Transpose();                                
                if (layerDeltaWeights_ != deltaWeights_) // If not same shape
                    layerDeltaWeights_.InitializeWithZeros(deltaWeights_.GetRowsCount(), deltaWeights_.GetColsCount());

                auto& deltaBias_ = error_.Transpose();
                if (layerDeltaBias_ != deltaBias_)
                    layerDeltaBias_.InitializeWithZeros(deltaBias_.GetRowsCount(),  deltaBias_.GetColsCount());
                
                layerDeltaWeights_ += deltaWeights_;
                layerDeltaBias_ += deltaBias_;
            }

            layerDeltaWeights_ = layerDeltaWeights_ * (learningRate / batchSize);
            layerDeltaBias_ = layerDeltaBias_ * (learningRate / batchSize);
            
            currentLayer_->UpdateWeights(layerDeltaWeights_, layerDeltaBias_);
        }
    }   

/* Constructors & Destructor */
public:
    Network() {
        // Dummy input_ must be created so that input layer can be initialized
        input_ = std::make_unique<Matrix<T>>(0, 0);

        lossFunction_ = std::unique_ptr<Functions::LossFunctionInterface<T>>(new LossFunctionType()); 

        // layers_ are not yet initialized (i.e. user just created an instance of network and wants to add layers)
        // input layer must be the first layer
        std::unique_ptr<Layers::LayerInterface<T>> inputLayer_(new Layers::InputLayer<T>((*input_)));
        layers_.push_back(std::move(inputLayer_));
    }   

    // Move constructor
    Network(Network<T, LossFunctionType>&& n) {
        this->layers_ = std::move(n.layers_);
        this->lossFunction_ = std::move(n.lossFunction_);
        this->input_ = std::move(n.input_);
    }

    ~Network() { }


/* Public Methods */
public:
    template <typename LayerType>
    const std::unique_ptr<Layers::LayerInterface<T>>& AddLayer(const std::size_t outputSize) {
        const auto& previousLayer_ = layers_.back();
        auto& previousOutput_ = previousLayer_->GetOutput();
        
        std::unique_ptr<Layers::LayerInterface<T>> newLayer_(new LayerType(previousOutput_, outputSize));        

        layers_.push_back(std::move(newLayer_));

        return layers_.back();
    }

    // Returns loss and predictions
    T_networkOutput Train(const Matrix<T>& input, const Matrix<T>& expectedOutput) {
        // One iteration is going forward and backward
        const auto& targets_ = RunIteration_(input, expectedOutput);        
        return targets_;
    }

    // Returns loss and predictions
    T_networkOutput Evaluate(const Matrix<T>& input, const Matrix<T>& expectedOutput) {
        const auto& targets_ = Forward_(input, expectedOutput);
        return targets_;
    }
};

