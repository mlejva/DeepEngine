#pragma once

#include "../Layers/LayerInterface.h"
#include "../Functions/ActivationFunctions/IdentityActivationFunction.h"

namespace Layers {
    template <typename T>
    class IdentityLayer : public Layers::LayerInterface<T> {
    private:
        typedef Layers::LayerInterface<T> base;
    
    /* Constructors & Destructor */
    public:        
        IdentityLayer(Matrix<T>& input, const std::size_t& outputSize) : base(input, outputSize) { 
            SetActivationFunction_(); 
        }
        ~IdentityLayer() { }

    /* Public Methods */      
    private:
        virtual void SetActivationFunction_() {
            base::activationFunction_ = std::make_unique<Functions::IdentityActivationFunction<T>>();
        }        
    };
}