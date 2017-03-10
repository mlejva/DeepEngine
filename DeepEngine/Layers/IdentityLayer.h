#pragma once

#include <memory>
#include <iostream>
#include "Layers/LayerInterface.h"
#include "Functions/ActivationFunctions/IdentityActivationFunction.h"

namespace Layers {
    template <typename T>
    class IdentityLayer : public Layers::LayerInterface<T> {
    private:
        typedef Layers::LayerInterface<T> base;
    
    /* Constructors & Destructor */
    public:
        IdentityLayer() { 
            SetActivationFunction(); 
        }
        IdentityLayer(const Matrix<T>& input) : Layers::LayerInterface<T>(input) { 
            SetActivationFunction(); 
        }

    /* Overrides */      
    private:
        virtual void SetActivationFunction() {
            base::activationFunction_ = std::make_unique<Functions::IdentityActivationFunction<T>>();
        }        
    };
}