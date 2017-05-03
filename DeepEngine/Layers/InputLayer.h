#pragma once

#include <memory>

#include "../Layers/LayerInterface.h"
#include "../Functions/ActivationFunctions/IdentityActivationFunction.h"

namespace Layers {
    template <typename T>
    class InputLayer : public Layers::LayerInterface<T> {
    private:
        typedef Layers::LayerInterface<T> base;
    
    /* Constructors & Destructor */
    public:        
        InputLayer(Matrix<T>& input) : base(input, 0, true) { 
            SetActivationFunction_(); 
        }
    
    /* Overrides */      
    private:
        virtual void SetActivationFunction_() {
            base::activationFunction_ = std::make_unique<Functions::IdentityActivationFunction<T>>();
        }        
    };
}