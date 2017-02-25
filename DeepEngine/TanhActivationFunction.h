#pragma once
#include <cmath>
#include "ActivationFunctionInterface.h"

template <typename T>
class TanhActivationFunction : public ActivationFunctionInterface<T>
{
public:

	TanhActivationFunction()
	{
	}

	~TanhActivationFunction()
	{
	}

	T apply(T& param) {
		return tanh(param);
	}
};

