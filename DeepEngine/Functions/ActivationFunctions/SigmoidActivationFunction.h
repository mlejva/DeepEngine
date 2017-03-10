#pragma once

#include <cmath>
#include "Functions/ActivationFunctions/ActivationFunctionInterface.h"

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
	};
}
