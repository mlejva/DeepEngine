#pragma once

#include "Matrix.h"
#include "Functions/FunctionInterface.h"

template <typename T>
class ActivationFunctionInterface : public FunctionInterface<T>
{
public:
	ActivationFunctionInterface() { }
	virtual ~ActivationFunctionInterface() { }

	virtual T apply(T& param) = 0;
};

