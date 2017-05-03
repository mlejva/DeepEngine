#pragma once

#include <cmath>

#include "../../Matrix.h"
#include "../../Functions/LossFunctions/LossFunctionInterface.h"

/*
	Computes Mean Squared Error loss across all elements of both matrices.
	Both matrices must have same shape.
*/

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
			if (predicted != expected) 
				throw MSELossException("Cannot compute MSE loss on two matrices of a different shape.");
			
			T mse_ = 0;
			for (std::size_t row = 0; row < predicted.GetRowsCount(); ++row) {
				for (std::size_t col = 0; col < predicted.GetColsCount(); ++col) {
					// Loss per example:
					// 1/2 * (predicted_i - expected_i)^2
					mse_ += (1.0 / 2.0) * pow((predicted(row, col) - expected(row, col)), 2);
				}
			}

			const auto& totalElements_ = predicted.GetRowsCount() * predicted.GetColsCount();
			T loss_ = /*(1.0 / totalElements_) * */ mse_; // 1/n * sum((predicted_i - expected_i)^2)
			return loss_;			
		}

		// Computes the loss function gradient with respect to the predicted values
		virtual Matrix<T> ComputeGradient(const Matrix<T>& predicted, const Matrix<T>& expected) {
			if (predicted != expected) 
				throw MSELossException("Cannot compute MSE derivative on two matrices of a different shape.");
			
			// Vector of the partial derivatives of MSE with respect to each expected value
			Matrix<T> gradientMatrix_(predicted.GetRowsCount(), predicted.GetColsCount());

			for (std::size_t row = 0; row < predicted.GetRowsCount(); ++row) {
				for (std::size_t col = 0; col < predicted.GetColsCount(); ++col) {
					T derivative_ = (predicted(row, col) - expected(row, col)); // dLoss/dExpected_i = (predicted_i - expected_i)
					gradientMatrix_(row, col) = derivative_;					
				}
			}

			return gradientMatrix_;
		}
	};
}
