#pragma once

#include <memory>

#include "../Layers/LayerInterface.h"
#include "../Functions/ActivationFunctions/LReluActivationFunction.h"

namespace Layers {
    template <typename T>
    class LReluLayer : public Layers::LayerInterface<T> {
    private:
        typedef Layers::LayerInterface<T> base;
    
    /* Constructors & Destructor */
    public:
        LReluLayer(Matrix<T>& input, const std::size_t& outputSize) : base(input, outputSize) { 
            SetActivationFunction_(); 
        }   
        ~LReluLayer() { }     

    /* Public Methods */      
    private:
        virtual void SetActivationFunction_() {
            base::activationFunction_ = std::make_unique<Functions::LReluActivationFunction<T>>();
        }
    };
}