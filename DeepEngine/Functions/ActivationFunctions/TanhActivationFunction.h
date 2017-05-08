#pragma once

#include <cmath>
#include <algorithm>
#include "../../Matrix.h"
#include "../../Functions/ActivationFunctions/ActivationFunctionInterface.h"

namespace Functions {
	template <typename T>
	class TanhActivationFunction : public Functions::ActivationFunctionInterface<T> {
	/* Constructors & Destructor */
	public:
		TanhActivationFunction() { }
		~TanhActivationFunction() { }

	/* Public Methods */
	public:
		T Apply(const T& param) {
			return tanh(param);
		}

		Matrix<T> Derivative(Matrix<T>& param) {
            std::for_each(param.GetDataBegin(), param.GetDataEnd(), [&](T& el_) {            
				// 1 - tanh^2(x)			
				const T th_ = tanh(el_);
    			el_ = 1 - (th_ * th_);
            });
			return param;
		}		
	};
}
