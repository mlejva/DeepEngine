#pragma once
#include <algorithm>
#include "ActivationFunctionInterface.h"

template <typename T>
class ReluActivationFunction : public ActivationFunctionInterface<T>
{
public:

	ReluActivationFunction()
	{
	}

	~ReluActivationFunction()
	{
	}

	T apply(T& param) {
		return std::max<T>(0, param);
	}
};

