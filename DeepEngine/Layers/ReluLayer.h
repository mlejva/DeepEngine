#pragma once

#include <memory>
#include "Layers/LayerInterface.h"
#include "Functions/ActivationFunctions/ReluActivationFunction.h"

namespace Layers {
    template <typename T>
    class ReluLayer : public Layers::LayerInterface<T> {
    private:
        typedef Layers::LayerInterface<T> base;
    
    /* Constructors & Destructor */
    public:
        ReluLayer() { SetActivationFunction(); }
        ReluLayer(const Matrix<T>& input) : Layers::LayerInterface<T>(input) { SetActivationFunction(); }

    /* Overrides */      
    private:
        virtual void SetActivationFunction() {
            base::activationFunction_ = std::make_unique<Functions::ReluActivationFunction<T>>();
        }
    };
}