#pragma once

#include <sstream> // TODO: DELETE

#include "Matrix.h"
#include "Vector.h"
#include "Functions/ActivationFunctions/ActivationFunctionInterface.h"

namespace Layers {    
    template <typename T>
    class LayerInterface {

    /* Private Methods */
    private:
        void SetLayer_(const Matrix<T>& input, const std::size_t& outputSize) {
            if (isInputLayer_) {
                output_.InitializeWithZeros(input_.GetRowsCount(), input_.GetColsCount());
            }
            else {
                output_.InitializeWithZeros(input_.GetRowsCount(), outputSize);
                weights_.RandomInitialization(input_.GetColsCount(), outputSize);
                bias_.InitializeWithZeros(outputSize);
            }    
        }

    /* Constructors For Children */
    protected:
        LayerInterface(const bool& isInputLayer = false) : input_(0, 0), weights_(0, 0), bias_(0), output_(0, 0), isInputLayer_(isInputLayer) { }
        LayerInterface(const Matrix<T>& input, const std::size_t& outputSize, const bool& isInputLayer = false) : 
            input_(input), 
            isInputLayer_(isInputLayer) {

            SetLayer_(input_, outputSize);
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
                Matrix<T> tmp = input_ * weights_;
                output_ = tmp;// + bias_; // z_n
            }
            output_.ApplyFunctionElementWise(activationFunction_); // a_n
        }

    /* Destructor */
    public:
        virtual ~LayerInterface() { }

    /* Getters & Setters */
    public:        
        void Run() { 
            ApplyActivationFunction_(); 
        }
        void Initialize(const Matrix<T>& input, const std::size_t& outputSize) {
            SetLayer_(input, outputSize);
        }
        const Matrix<T>& GetOutput() { return output_; }
        const Matrix<T>& GetInput() { return input_; }

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
