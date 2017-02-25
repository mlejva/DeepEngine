#pragma once
#include "FunctionInterface.h"
#include "Matrix.h"

template <typename T>
class LossFunctionInterface : public FunctionInterface<T>
{
public:

	LossFunctionInterface()
	{
	}

	virtual ~LossFunctionInterface()
	{
	}

	virtual Matrix<T> apply(Matrix<T>& predicted, Matrix<T>& expected) = 0;
};

