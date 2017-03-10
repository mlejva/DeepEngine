#pragma once

#include <cmath>
#include "Functions/ActivationFunctions/ActivationFunctionInterface.h"

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
	};
}
