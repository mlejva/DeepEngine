#pragma once

#include <algorithm>
#include "../../Matrix.h"
#include "../../Functions/ActivationFunctions/ActivationFunctionInterface.h"

namespace Functions {
	template <typename T>
	class ReluActivationFunction : public Functions::ActivationFunctionInterface<T> {
	/* Constructors & Destructor */
	public:
		ReluActivationFunction() { }
		~ReluActivationFunction() { }

	/* Public Methods */
	public:
		T Apply(const T& param) {
			return std::max<T>(0, param);
		}

		Matrix<T> Derivative(Matrix<T>& param) {
			//Matrix<T> newMatrix_(param)
            std::for_each(param.GetDataBegin(), param.GetDataEnd(), [&](T& el_) {
                if (el_ > 0.0)
					el_ = 1.0;
				else
					el_ = 0.0;
            });
			
			return param;
		}

		/*
		T ComputeDerivative(const T& param) {
			if (param > 0.0)
				return 1.0;
			else
				return 0.0;
		}
		*/
	};
}
