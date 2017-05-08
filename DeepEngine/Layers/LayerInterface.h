#pragma once

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

        std::unique_ptr<Functions::ActivationFunctionInterface<T> > activationFunction_;

        bool isInputLayer_;
        bool areWeightsInitialized_;
   
    /* Constructors */
    protected:
        LayerInterface(Matrix<T>& input, const std::size_t outputSize, const bool& isInputLayer = false, const bool& areWeightsInitialized = false) : 
            input_(input),
            outputSize_(outputSize),
            isInputLayer_(isInputLayer),
            areWeightsInitialized_(areWeightsInitialized) { }
    
    /* Destructor */
    public:
        virtual ~LayerInterface() { }

    /* Base & Children Methods */
    protected:
        virtual void SetActivationFunction_() = 0;
        void ApplyActivationFunction_() {                  
            if (isInputLayer_) {
                zValue_.ReshapeWithMatrix(input_);
                output_ = zValue_;
            }
            else {
                // Initialize weights while traversing layers for the first time
                if (!areWeightsInitialized_) {
                    const std::size_t& layerInputSize_ = input_.GetRowsCount();                    
                    weights_.XavierInitialization(input_.GetColsCount(), outputSize_, layerInputSize_, outputSize_); 
                    bias_.InitializeWithOnes(1, outputSize_);                    

                    areWeightsInitialized_ = true;   
                }
                zValue_.ReshapeWithMatrix(input_ * weights_);

                output_ = zValue_;
                for (std::size_t row = 0; row < output_.GetRowsCount(); ++row) {
                    for (std::size_t col = 0; col < output_.GetColsCount(); ++col) {
                        T& el_ = output_(row, col);                        
                        el_ = activationFunction_->Apply(el_ + bias_(0, col));
                    }
                }
            }                       
        }

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
        }
        
        const Matrix<T> ComputeLayerError(const Matrix<T>& previousLayerError, const std::size_t& inputIndex) {                   
            // Compute derivative of activation function with zValue_       
            auto zValueForSingleInput_ = zValue_.GetRow(inputIndex).Transpose();  
            const auto& actDerivative_ = activationFunction_->Derivative(zValueForSingleInput_);            

            Matrix<T> layerError_ = Matrix<T>::Multiply(previousLayerError, actDerivative_);            
            return layerError_;
        }
        
        void UpdateWeights(const Matrix<T>& deltaWeights, const Matrix<T>& deltaBias) {                       
            weights_ -= deltaWeights;   
            bias_ -= deltaBias;                                             
        }      
    };
}
