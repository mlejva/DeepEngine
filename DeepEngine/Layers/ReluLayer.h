#pragma once

#include <memory>

#include "../Layers/LayerInterface.h"
#include "../Functions/ActivationFunctions/ReluActivationFunction.h"

namespace Layers {
    template <typename T>
    class ReluLayer : public Layers::LayerInterface<T> {
    private:
        typedef Layers::LayerInterface<T> base;
    
    /* Constructors & Destructor */
    public:
        ReluLayer(Matrix<T>& input, const std::size_t& outputSize) : base(input, outputSize) { 
            SetActivationFunction_(); 
        }
        ~ReluLayer() { }

    /* Public Methods */      
    private:
        virtual void SetActivationFunction_() {
            base::activationFunction_ = std::make_unique<Functions::ReluActivationFunction<T>>();
        }
    };
}