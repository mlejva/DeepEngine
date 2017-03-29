#pragma once

#include "Functions/ActivationFunctions/ActivationFunctionInterface.h"

namespace Functions {
    template <typename T>
    class IdentityActivationFunction : public Functions::ActivationFunctionInterface<T> {
    /* Constructors & Destructor */
    public:
        IdentityActivationFunction() { }
        ~IdentityActivationFunction() { }

    /* Public Methods */
    public:
        T Apply(const T& param) {
            return param;
        }

        Matrix<T> Derivative(Matrix<T>& param) {
            return param;
        }        
    };
}
