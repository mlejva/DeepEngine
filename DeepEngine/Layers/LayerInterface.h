#pragma once

// TODO: DELETE
#include <sstream> 
#include <iostream>

#include "Matrix.h"
#include "Vector.h"
#include "Functions/ActivationFunctions/ActivationFunctionInterface.h"

namespace Layers {    
    template <typename T>
    class LayerInterface {

    /* Private Methods */
    private:
        void SetLayer_(const std::size_t& outputSize) {
            if (isInputLayer_) {
                output_.InitializeWithZeros(input_.GetRowsCount(), input_.GetColsCount());
            }
            else {
                output_.InitializeWithZeros(input_.GetRowsCount(), outputSize);
                weights_.RandomInitialization(input_.GetColsCount(), outputSize);
                std::cout << "Weights: " << std::endl;
                std::cout << weights_ << std::endl;

                bias_.InitializeWithZeros(outputSize);
            }    
        }

    /* Constructors */
    protected:
        LayerInterface(const bool& isInputLayer = false) : input_(0, 0), weights_(0, 0), bias_(0), output_(0, 0), isInputLayer_(isInputLayer) { }
        LayerInterface(const Matrix<T>& input, const std::size_t& outputSize, const bool& isInputLayer = false) : 
            input_(input), 
            isInputLayer_(isInputLayer) {

            SetLayer_(outputSize);
        }

    /* Base & Children Properties */
    protected:
        const Matrix<T>& input_;
        Matrix<T> output_;
        Matrix<T> weights_;
        Vector<T> bias_;

        std::unique_ptr<Functions::ActivationFunctionInterface<T>> activationFunction_;

        bool isInputLayer_;
    
    /* Base & Children Methods */
    protected:
        virtual void SetActivationFunction_() = 0;
        void ApplyActivationFunction_() {       
            if (isInputLayer_) {
                output_ = input_;
            }
            else {
                output_ = input_ * weights_;// + bias_; // z_n
            }
            output_.ApplyFunctionElementWise(activationFunction_); // a_n
        }

    /* Destructor */
    public:
        virtual ~LayerInterface() { }

    /* Getters & Setters */
    public:        
        const Matrix<T>& GetOutput() { return output_; }
        const Matrix<T>& GetInput() { return input_; }
        //void SetInput(const Matrix<T>& input) { input_.ResizeWithMatrix(input); } 
    
    /* Public Methods */
    public:
        void Forward() { 
            ApplyActivationFunction_(); 
        }
        void Initialize(const Matrix<T>& input, const std::size_t& outputSize) {
            SetLayer_(input, outputSize);
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
