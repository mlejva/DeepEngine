#pragma once

#include <algorithm>
#include "../../Matrix.h"
#include "../../Functions/ActivationFunctions/ActivationFunctionInterface.h"

// Leaky ReLU
namespace Functions {
	template <typename T>
	class LReluActivationFunction : public Functions::ActivationFunctionInterface<T> {
	/* Constructors & Destructor */
	public:
		LReluActivationFunction() { }
		~LReluActivationFunction() { }

	/* Public Methods */
	public:
		T Apply(const T& param) {
			return std::max<T>(0, param);
		}

		Matrix<T> Derivative(Matrix<T>& param, double threshold = 0.01) {
            std::for_each(param.GetDataBegin(), param.GetDataEnd(), [&](T& el_) {
                if (el_ > 0.0)
					el_ = 1.0;
				else
					el_ = threshold;
            });			
			return param;
		}
	};
}
