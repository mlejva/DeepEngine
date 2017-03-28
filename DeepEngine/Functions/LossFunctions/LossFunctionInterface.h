#pragma once

#include "Matrix.h"

namespace Functions {
	template <typename T>
	class LossFunctionInterface {	
	/* Constructors & Destructor */
	public:
		LossFunctionInterface() { }
		virtual ~LossFunctionInterface() { }

	/* Public Methods */
	public:
		virtual T Apply(const Matrix<T>& predicted, const Matrix<T>& expected) = 0;
	};
}
