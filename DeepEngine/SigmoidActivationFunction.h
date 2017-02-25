#pragma once
#include <cmath>
#include "ActivationFunctionInterface.h"

template <typename T>
class SigmoidActivationFunction : ActivationFunctionInterface<T>
{
public:

	SigmoidActivationFunction()
	{
	}

	~SigmoidActivationFunction()
	{
	}

	T apply(T& param) {
		return 1 / (1 + exp(-param));
	}
};

