#pragma once

#include "Matrix.h"
#include "Functions/ActivationFunctions/ActivationFunctionInterface.h"

namespace Layers {
    template <typename T>
    class LayerInterface {
    /* Constructors For Children */
    protected:
        LayerInterface() { }
        LayerInterface(const Matrix<T>& input) : input_(input) {  }

    /* Base & Children Properties */
    protected:
        Matrix<T> input_;
        Matrix<T> output_;
        std::unique_ptr<Functions::ActivationFunctionInterface<T>> activationFunction_;
    
    /* Base & Children Methods */
    protected:
        virtual void SetActivationFunction() = 0;
        void ApplyActivationFunction() {
            output_ = input_;
            output_.ApplyFunctionElementWise(activationFunction_);
        }

    /* Destructor */
    public:
        virtual ~LayerInterface() { }

    /* Getters & Setters */
    public:
        void SetInput(const Matrix<T>& input) { input_ = input; }
        const Matrix<T>& GetOutput() { ApplyActivationFunction(); return output_; }
        const Matrix<T>& GetInput() { return input_; }
    };
}
