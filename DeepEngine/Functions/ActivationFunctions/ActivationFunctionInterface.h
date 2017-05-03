#pragma once

#include <algorithm>
#include "../../Matrix.h"

namespace Functions {
	template <typename T>
	class ActivationFunctionInterface {
	/* Constructor & Destructor */
	public:
		ActivationFunctionInterface() { }
		virtual ~ActivationFunctionInterface() { }

	/* Public Methods */
	public:
		virtual T Apply(const T& param) = 0;
		virtual Matrix<T> Derivative(Matrix<T>& param) = 0;
	};
}
