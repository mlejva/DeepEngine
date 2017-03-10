#pragma once

#include "Layers/LayerInterface.h"
#include "Functions/ActivationFunctions/SigmoidActivationFunction.h"

namespace Layers {
    template <typename T>
    class SigmoidLayer : public Layers::LayerInterface<T> {
    private:
    typedef Layers::LayerInterface<T> base;
    
    /* Constructors & Destructor */
    public:
        SigmoidLayer() { 
            SetActivationFunction(); 
        }
        SigmoidLayer(const Matrix<T>& input) : Layers::LayerInterface<T>(input) { 
            SetActivationFunction(); 
        }

    /* Overrides */      
    private:
        virtual void SetActivationFunction() {
            base::activationFunction_ = std::make_unique<Functions::SigmoidActivationFunction<T>>();
        }        
    };    
}
