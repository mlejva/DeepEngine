#pragma once
#include "Matrix.h"
#include "FunctionInterface.h"

template <typename T>
class ActivationFunctionInterface : FunctionInterface<T>
{
public:

	ActivationFunctionInterface()
	{
	}

	virtual ~ActivationFunctionInterface()
	{
	}

	virtual T apply(T& val) = 0;
};

