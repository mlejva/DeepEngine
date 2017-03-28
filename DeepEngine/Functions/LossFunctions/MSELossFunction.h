#pragma once

#include <cmath>

#include "Matrix.h"
#include "Functions/LossFunctions/LossFunctionInterface.h"

namespace Functions {
	class MSELossException : public std::runtime_error {
	public:
		MSELossException(const std::string& message) : runtime_error(message) { }
	};

	template <typename T>
	class MSELossFunction : public Functions::LossFunctionInterface<T> {
	private:
		typedef Functions::LossFunctionInterface<T> base;

	/* Cosntructors & Destructor */
	public:
		MSELossFunction() : base()  { }
		~MSELossFunction() { }

	/* Public Methdos */
	public:
		virtual T Apply(const Matrix<T>& predicted, const Matrix<T>& expected) {
			if (predicted.GetRowsCount() > 1 || expected.GetRowsCount() > 1)
				throw MSELossException("MSE loss expects single row for both matrices.");

			T mse_ = 0;
			for (auto i = 0; i < predicted.GetRowsCount(); ++i) {
				mse_ += pow((predicted(i, 0) - expected(i, 0)), 2);
			}
			return (1.0 / 2.0 * predicted.GetRowsCount()) * mse_;
			//return (1.0 / 2.0) * mse_;
		}
	};
}
