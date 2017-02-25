#pragma once
#include <algorithm>
#include "ActivationFunctionInterface.h"

template <typename T>
class ReluActivationFunction : ActivationFunctionInterface<T>
{
public:

	ReluActivationFunction()
	{
	}

	virtual ~ReluActivationFunction()
	{
	}

	T apply(T& val) {
		return std::max<T>(0, val);
	}
};

