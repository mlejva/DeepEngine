#pragma once

/*#include <stdio.h>
#include <fstream>
#include <cmath>*/
#include <iostream> // TODO: Delete
#include <algorithm>
#include <random>
#include <string>
#include <vector>
#include <exception>
#include "FunctionInterface.h"

class WrongMatrixDimensionException : public std::runtime_error {
public:
	WrongMatrixDimensionException(const std::string& message) : runtime_error(message) { }
};

class InvalidIndexException : public std::runtime_error {
public:
	InvalidIndexException(const std::string& message) : runtime_error(message) { }
};

template <typename T>
class Matrix {
	/* Constructors & Destructor */
	public:
		Matrix() { ResizeMatrix_(0, 0); }
		Matrix(const std::size_t& rows, const std::size_t& cols) { ResizeMatrix_(rows, cols); }
		Matrix(const Matrix<T>& m) : 
			rowsCount_(m.rowsCount_), 
			colsCount_(m.colsCount_) { data_ = std::vector<T>(m.data_); }		

		~Matrix() { }
	
	/* Operators */
	public:
		T& operator() (const std::size_t& rowPos, const std::size_t& colPos) {			
			if (rowPos > rowsCount_ || colPos > colsCount_)
				throw InvalidIndexException("Index you are trying to access is not valid.");

			return data_[colsCount_ * rowPos + colPos];
		}

		const T& operator() (const std::size_t& rowPos, const std::size_t& colPos) const {			
			if (rowPos > rowsCount_ || colPos > colsCount_)
				throw InvalidIndexException("Index you are trying to access is not valid.");

			return data_[colsCount_ * rowPos + colPos];
		}
	
		const Matrix<T>& operator= (const Matrix<T>& m) {
			if (this->rowsCount_ == m.rowsCount_ && this->colsCount_ == m.colsCount_) {
				this->data_ = std::vector<int>(m.data_);
				return *this;
			}
			else {
				throw WrongMatrixDimensionException("Both matrices must have same dimensions.");
			}
		}		

		friend std::ostream& operator<<(std::ostream& s, const Matrix<T>& m) {
			for (std::size_t row = 0; row < m.rowsCount_; ++row) {
				for (std::size_t col = 0; col < m.colsCount_; ++col) {
					if (col != m.colsCount_ - 1)
						s << m(row, col) << "\t";
					else
						s << m(row, col);
				}
				s << std::endl;
			}
			return s;
		}

	/* Matrix-Matrix Operators */
	public:
		Matrix<T> operator+ (const Matrix<T>& m) {
			if (this->rowsCount_ == m.rowsCount_ && this->colsCount_ == m.colsCount_) {
				Matrix<T> newMatrix_(this->rowsCount_, this->colsCount_);
				for (std::size_t row = 0; row < this->rowsCount_; ++row) {
					for (std::size_t column = 0; column < this->colsCount_; ++column) {
						newMatrix_(row, column) = (*this)(row, column) + m(row, column);
					}
				}
				return newMatrix_;
			}
			else {
				throw WrongMatrixDimensionException("Both matrices must have same dimensions.");
			}
		}

		const Matrix<T> operator+ (const Matrix<T>& m) const {
			if (this->rowsCount_ == m.rowsCount_ && this->colsCount_ == m.colsCount_) {
				Matrix<T> newMatrix_(this->rowsCount_, this->colsCount_);
				for (std::size_t row = 0; row < this->rowsCount_; ++row) {
					for (std::size_t column = 0; column < this->colsCount_; ++column) {
						newMatrix_(row, column) = (*this)(row, column) + m(row, column);
					}
				}
				return newMatrix_;
			}
			else {
				throw WrongMatrixDimensionException("Both matrices must have same dimensions.");
			}
		}		

		Matrix<T> operator- (const Matrix<T>& m) {
			if (this->rowsCount_ == m.rowsCount_ && this->colsCount_ == m.colsCount_) {
				Matrix<T> newMatrix_(this->rowsCount_, this->colsCount_);
				for (std::size_t row = 0; row < this->rowsCount_; ++row) {
					for (std::size_t column = 0; column < this->colsCount_; ++column) {
						newMatrix_(row, column) = (*this)(row, column) - m(row, column);
					}
				}
				return newMatrix_;
			}
			else {
				throw WrongMatrixDimensionException("Both matrices must have same dimensions.");
			}
		}

		const Matrix<T> operator- (const Matrix<T>& m) const {
			if (this->rowsCount_ == m.rowsCount_ && this->colsCount_ == m.colsCount_) {
				Matrix<T> newMatrix_(this->rowsCount_, this->colsCount_);
				for (std::size_t row = 0; row < this->rowsCount_; ++row) {
					for (std::size_t column = 0; column < this->colsCount_; ++column) {
						newMatrix_(row, column) = (*this)(row, column) - m(row, column);
					}
				}
				return newMatrix_;
			}
			else {
				throw WrongMatrixDimensionException("Both matrices must have same dimensions.");
			}
		}

		Matrix<T> operator* (const Matrix<T>& m) {
			if (this->colsCount_ != m.rowsCount_)				
				throw WrongMatrixDimensionException("The left-hand matrix must have the same width as is the height of the right-hand matrix.");

			Matrix<T> newMatrix_(this->rowsCount_, m.colsCount_);
			for (std::size_t row = 0; row < newMatrix_.rowsCount_; ++row) {
				for (std::size_t column = 0; column < newMatrix_.colsCount_; ++column) {
					T newValue_ = 0;
					for (std::size_t inner = 0; inner < this->colsCount_; ++inner) {
						newValue_ += (*this)(row, inner) * m(inner, column);
					}
				}
			}
			return newMatrix_;
		}

		const Matrix<T> operator* (const Matrix<T>& m) const {
			if (this->colsCount_ != m.rowsCount_)			
				throw WrongMatrixDimensionException("The left-hand matrix must have the same width as is the height of the right-hand matrix.");			

			Matrix<T> newMatrix_(this->rowsCount_, m.colsCount_);
			for (std::size_t row = 0; row < newMatrix_.rowsCount_; ++row) {
				for (std::size_t column = 0; column < newMatrix_.colsCount_; ++column) {
					T newValue_ = 0;
					for (std::size_t inner = 0; inner < this->colsCount_; ++inner) {
						newValue_ += (*this)(row, inner) * m(inner, column);
					}
				}
			}
			return newMatrix_;
		}
	
	/* Matrix-Scalar Operators */
	public:
		Matrix<T> operator+ (const T& s) {
			Matrix<T> newMatrix_(this->rowsCount_, this->colsCount_);
			for (std::size_t row = 0; row < this->rowsCount_; ++row) {
				for (std::size_t column = 0; column < this->colsCount_; ++column) {
					newMatrix_(row, column) = (*this)(row, column) + s;
				}
			}
			return newMatrix_;
		}

		const Matrix<T> operator+ (const T& s) const {
			Matrix<T> newMatrix_(this->rowsCount_, this->colsCount_);
			for (std::size_t row = 0; row < this->rowsCount_; ++row) {
				for (std::size_t column = 0; column < this->colsCount_; ++column) {
					newMatrix_(row, column) = (*this)(row, column) + s;
				}
			}
			return newMatrix_;
		}

		Matrix<T> operator- (const T& s) {
			Matrix<T> newMatrix_(this->rowsCount_, this->colsCount_);
			for (std::size_t row = 0; row < this->rowsCount_; ++row) {
				for (std::size_t column = 0; column < this->colsCount_; ++column) {
					newMatrix_(row, column) = (*this)(row, column) - s;
				}
			}
			return newMatrix_;
		}

		const Matrix<T> operator- (const T& s) const {
			Matrix<T> newMatrix_(this->rowsCount_, this->colsCount_);
			for (std::size_t row = 0; row < this->rowsCount_; ++row) {
				for (std::size_t column = 0; column < this->colsCount_; ++column) {
					newMatrix_(row, column) = (*this)(row, column) - s;
				}
			}
			return newMatrix_;
		}

		Matrix<T> operator* (const T& s) {
			Matrix<T> newMatrix_(this->rowsCount_, this->colsCount_);
			for (std::size_t row = 0; row < this->rowsCount_; ++row) {
				for (std::size_t column = 0; column < this->colsCount_; ++column) {
					newMatrix_(row, column) = (*this)(row, column) * s;
				}
			}
			return newMatrix_;
		}

		const Matrix<T> operator* (const T& s) const {
			Matrix<T> newMatrix_(this->rowsCount_, this->colsCount_);
			for (std::size_t row = 0; row < this->rowsCount_; ++row) {
				for (std::size_t column = 0; column < this->colsCount_; ++column) {
					newMatrix_(row, column) = (*this)(row, column) * s;
				}
			}
			return newMatrix_;
		}

	/* Getters & Setters */
	public:
		const std::size_t& GetRowsCount() const { return rowsCount_; }
		const std::size_t& GetColsCount() const { return colsCount_; }

	/* Public methods */
	public:
		template <typename FuncType>
		void ApplyFunctionElementWise() { 
			std::for_each(this->begin(), this->end(), [](T& el_) {
				FuncType func_;
				el_ = func_.apply(el_);
			});
		}

		void RandomInitialization() {
			std::random_device rd_; // Obtain a random number from hardware
			// A Mersenne Twister pseudo-random generator of 32-bit numbers 
			// with a state size of 19937 bits.
			std::mt19937 gen_(rd_()); // Seed the generator
			// Xavier initialization
			T start_ = -(sqrt(6.0 / (this->rowsCount_ + this->colsCount_)));
			T end_ = (sqrt(6.0 / (this->rowsCount_ + this->colsCount_)));
			// TODO: Now works only normal distribution for real numbers
			std::uniform_real_distribution<> distr_(start_, end_); // Define the range
			
			std::for_each(this->data_.begin(), this->data_.end(), [&](T& el_) {
				el_ = distr_(gen_);
			});
		}
	private:
		std::size_t rowsCount_;
		std::size_t colsCount_;
		std::vector<T> data_;

		void ResizeMatrix_(std::size_t rows, std::size_t cols) { 
			rowsCount_ = rows;
			colsCount_ = cols;

			data_ = std::vector<T>(rowsCount_ * colsCount_); 
		}
};
