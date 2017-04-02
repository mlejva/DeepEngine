#pragma once

// TODO: DELETE
#include <sstream> 
#include <iostream>

#include <algorithm>
#include "Matrix.h"
#include "Functions/ActivationFunctions/ActivationFunctionInterface.h"

namespace Layers {    
    template <typename T>
    class LayerInterface {

    /* Private Methods */
    private:        
        void SetLayer_(const Matrix<T>& input, const std::size_t& outputSize) {            
            if (isInputLayer_) {
                output_.InitializeWithZeros(input.GetRowsCount(), input.GetColsCount());
            }
            else {
                output_.InitializeWithZeros(input.GetRowsCount(), outputSize);
                weights_.RandomInitialization(input.GetColsCount(), outputSize);                
                
                bias_.InitializeWithZeros(outputSize, 0);
            }    
        }

    /* Constructors */
    protected:
        LayerInterface(const bool& isInputLayer = false) : 
            input_(0, 0), 
            weights_(0, 0), 
            bias_(0), 
            output_(0, 0), 
            isInputLayer_(isInputLayer) { }
        LayerInterface(const Matrix<T>& input, const std::size_t& outputSize, const bool& isInputLayer = false) : 
            input_(input),
            outputSize_(outputSize),
            isInputLayer_(isInputLayer) { }

    /* Base & Children Properties */
    protected:
        const Matrix<T>& input_;
        const std::size_t& outputSize_;

        Matrix<T> output_;
        Matrix<T> weights_;
        Matrix<T> bias_;                

        Matrix<T> zValue_;

        std::unique_ptr<Functions::ActivationFunctionInterface<T>> activationFunction_;

        bool isInputLayer_;
    
    /* Base & Children Methods */
    protected:
        virtual void SetActivationFunction_() = 0;
        void ApplyActivationFunction_() {       
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
        const Matrix<T>& GetOutput() { return output_; }
        const Matrix<T>& GetInput() { return input_; }
        //void SetInput(const Matrix<T>& input) { input_.ResizeWithMatrix(input); } 
    
        const Matrix<T>& GetWeights() { return weights_; }
    /* Public Methods */
    public:
        void Forward() {
            SetLayer_(input_, outputSize_);
            ApplyActivationFunction_(); 
        }
        
        void Initialize(const Matrix<T>& input) {
            input_ = input;
            SetLayer_(input_, outputSize_);
        }
        
        const Matrix<T> ComputeLayerError(const Matrix<T>& previousLayerError, const std::size_t& inputIndex) {                   
            // Compute derivative of activation function with zValue_       
            auto zValueForSingleInput_ = zValue_.GetRow(inputIndex).Transpose();  
            const auto& actDerivative_ = activationFunction_->Derivative(zValueForSingleInput_);            

            Matrix<T> layerError_ = Matrix<T>::multiply(previousLayerError, actDerivative_);            
            return layerError_;
        }
        
        void UpdateWeights(const Matrix<T>& deltaWeights/*, const Matrix<T>& biasDelta*/) {            
            weights_ -= deltaWeights;            
        }

        // TODO: DELETE BOTH WHEN DONE
        const std::string GetLayerInputShape() {
            std::stringstream ss;
            ss << "(" << input_.GetRowsCount() << "x" << input_.GetColsCount() << ")";

            //const String& shape = "(" + input_.GetRowsCount() + "x" + input_.GetColsCount() + ")";
            return ss.str();
        }
        const std::string GetLayerOutputShape() {
            std::stringstream ss;
            ss << "(" << output_.GetRowsCount() << "x" << output_.GetColsCount() << ")";

            //const String& shape = "(" + input_.GetRowsCount() + "x" + input_.GetColsCount() + ")";
            return ss.str();
        }
    };
}
