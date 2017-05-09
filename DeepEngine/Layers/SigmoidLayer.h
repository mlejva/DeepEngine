#pragma once

#include "../Layers/LayerInterface.h"
#include "../Functions/ActivationFunctions/SigmoidActivationFunction.h"

namespace Layers {
    template <typename T>
    class SigmoidLayer : public Layers::LayerInterface<T> {
    private:
        typedef Layers::LayerInterface<T> base;
    
    /* Constructors & Destructor */
    public:
        SigmoidLayer(Matrix<T>& input, const std::size_t& outputSize) : base(input, outputSize) { 
            SetActivationFunction_(); 
        }
        ~SigmoidLayer() { }

    /* Public Methods */      
    private:
        virtual void SetActivationFunction_() {
            base::activationFunction_ = std::make_unique<Functions::SigmoidActivationFunction<T>>();
        }        
    };    
}
