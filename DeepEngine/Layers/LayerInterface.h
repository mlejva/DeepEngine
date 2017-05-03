#pragma once

// TODO: DELETE
#include <sstream> 
#include <iostream>

#include <algorithm>
#include "../Matrix.h"
#include "../Functions/ActivationFunctions/ActivationFunctionInterface.h"

namespace Layers {    
    template <typename T>
    class LayerInterface {

    /* Base & Children Properties */
    protected:
        Matrix<T>& input_;
        const std::size_t outputSize_;

        Matrix<T> output_;
        Matrix<T> weights_;
        Matrix<T> bias_;                

        Matrix<T> zValue_;

        std::unique_ptr<Functions::ActivationFunctionInterface<T>> activationFunction_;

        bool isInputLayer_;
        bool areWeightsInitialized_;
   
    /* Constructors */
    protected:
        LayerInterface(Matrix<T>& input, const std::size_t outputSize, const bool& isInputLayer = false, const bool& areWeightsInitialized = false) : 
            input_(input),
            outputSize_(outputSize),
            isInputLayer_(isInputLayer),
            areWeightsInitialized_(areWeightsInitialized) { }
    
    /* Base & Children Methods */
    protected:
        virtual void SetActivationFunction_() = 0;
        void ApplyActivationFunction_() {      
            if (!areWeightsInitialized_) {
                weights_.RandomInitialization(input_.GetColsCount(), outputSize_); 
                areWeightsInitialized_ = true;               
            }

            if (isInputLayer_) {
                zValue_.ReshapeWithMatrix(input_);
                //output_ = input_;
            }
            else {
                zValue_.ReshapeWithMatrix(input_ * weights_);// + bias_; // z_n
                //output_ = input_ * weights_;// + bias_; // z_n
            }            

            output_ = zValue_;
            std::for_each(output_.GetDataBegin(), output_.GetDataEnd(), [&](T& el_) {
                el_ = activationFunction_->Apply(el_);
            });
        }

    /* Destructor */
    public:
        virtual ~LayerInterface() { }

    /* Getters & Setters */
    public:        
        Matrix<T>& GetOutput() { return output_; }
        const Matrix<T>& GetInput() { return input_; }
    
        const Matrix<T>& GetWeights() { return weights_; }
    /* Public Methods */
    public:
        void Forward() {
            ApplyActivationFunction_(); 
        }
        
        void Initialize(const Matrix<T>& input) {
            input_ = input;
            if (isInputLayer_) {
                output_.InitializeWithZeros(input.GetRowsCount(), input.GetColsCount());
            }
        }
        
        const Matrix<T> ComputeLayerError(const Matrix<T>& previousLayerError, const std::size_t& inputIndex) {                   
            // Compute derivative of activation function with zValue_       
            auto zValueForSingleInput_ = zValue_.GetRow(inputIndex).Transpose();  
            const auto& actDerivative_ = activationFunction_->Derivative(zValueForSingleInput_);            

            Matrix<T> layerError_ = Matrix<T>::Multiply(previousLayerError, actDerivative_);            
            return layerError_;
        }
        
        void UpdateWeights(const Matrix<T>& deltaWeights/*, const Matrix<T>& biasDelta*/) {            
            weights_ -= deltaWeights;            
        }      
    };
}
