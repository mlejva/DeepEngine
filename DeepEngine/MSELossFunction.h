#pragma once
#include "Matrix.h"
#include "LossFunctionInterface.h"

template <typename T>
class MSELossFunction : public LossFunctionInterface<T>
{
public:

	MSELossFunction()
	{
	}

	~MSELossFunction()
	{
	}

	T apply(Matrix<T>& predicted, Matrix<T>& expected) {
		// TODO: Add exception if both matrices aren't vectors
		// TODO: Add exception if both matrices aren't same height
		
		T mse_ = 0;
		for (typename Matrix<T>::size_type i = 0; i < predicted.getHeight(); ++i) {
			mse_ += predicted.GetElement(i, 0) - expected.GetElement(i, 0);
		}
		return (1.0 / predicted.getHeight()) * mse_;
	}
};

