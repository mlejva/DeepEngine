#pragma once

#include <cmath>
#include <algorithm>
#include "../../Matrix.h"
#include "../../Functions/ActivationFunctions/ActivationFunctionInterface.h"

namespace Functions {
	template <typename T>
	class SigmoidActivationFunction : public Functions::ActivationFunctionInterface<T> {
	/* Constructors & Destructor */
	public:
		SigmoidActivationFunction() { }
		~SigmoidActivationFunction() { }

	/* Public Methods */
	public:
		T Apply(const T& param) {
			return 1 / (1 + exp(-param));
		}

		Matrix<T> Derivative(Matrix<T>& param) {
            std::for_each(param.GetDataBegin(), param.GetDataEnd(), [&](T& el_) {            
				const T sigmoid_ = 1.0 / (1 + exp(-el_));
				el_ = sigmoid_ * (1 - sigmoid_);
            });
			return param;
		}		
	};
}
