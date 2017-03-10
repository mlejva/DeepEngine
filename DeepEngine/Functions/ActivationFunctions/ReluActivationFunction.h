#pragma once

#include "Functions/ActivationFunctions/ActivationFunctionInterface.h"

namespace Functions {
	template <typename T>
	class ReluActivationFunction : public Functions::ActivationFunctionInterface<T> {
	/* Constructors & Destructor */
	public:
		ReluActivationFunction() { }
		~ReluActivationFunction() { }

	/* Public Methods */
	public:
		T Apply(const T& param) {
			return std::max<T>(0, param);
		}
	};
}
