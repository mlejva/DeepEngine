#pragma once

#include <memory>

#include "../Layers/LayerInterface.h"
#include "../Functions/ActivationFunctions/TanhActivationFunction.h"

namespace Layers {
    template <typename T>
    class TanhLayer : public Layers::LayerInterface<T> {
    private:
        typedef Layers::LayerInterface<T> base;
    
    /* Constructors & Destructor */
    public:
        TanhLayer(Matrix<T>& input, const std::size_t& outputSize) : base(input, outputSize) { 
            SetActivationFunction_(); 
        }
        ~TanhLayer() { }

    /* Public Methods */      
    private:
        virtual void SetActivationFunction_() {
            base::activationFunction_ = std::make_unique<Functions::TanhActivationFunction<T>>();
        }
    };
}