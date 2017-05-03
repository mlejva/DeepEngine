#pragma once

// TODO: Delete
#include <iostream>

#include <sstream>
#include <fstream>
#include <algorithm>
#include <random>
#include <string>
#include <vector>
#include <exception>

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
/* Public Static Methods */
public:
	/*// Computes a multiplication between two matrices
	static Matrix<T> dot(const Matrix<T>& left, const Matrix<T>& right) {

	}*/
	
	// Computes a Hadamard product of two matrices
	static Matrix<T> Multiply(const Matrix<T>& left, const Matrix<T>& right) {
		if (left != right)
			throw WrongMatrixDimensionException("Both matrices must have the same shape.");
		
		const auto& rowsTotal_ = left.GetRowsCount();
		const auto& colsTotal_ = left.GetColsCount();

		Matrix<T> newMatrix_(rowsTotal_, colsTotal_);
		for (std::size_t row = 0; row < rowsTotal_; ++row) {
			for (std::size_t column = 0; column < colsTotal_; ++column) {
				newMatrix_(row, column) = left(row, column) * right(row, column);
			}
		}
		return newMatrix_;
	}

/* Private Properties */
private:
	std::size_t rowsCount_;
	std::size_t colsCount_;
	std::vector<T> data_;

/* Private Methods */
private: 
	void ResizeMatrix_(std::size_t rows, std::size_t cols) { 
		rowsCount_ = rows;
		colsCount_ = cols;

		data_ = std::vector<T>(rowsCount_ * colsCount_);
	}

	void AddRow_(const std::vector<T>& newRow) {
		rowsCount_++;

		if (newRow.size() != colsCount_) {
			throw WrongMatrixDimensionException("You are trying to add a row with the length of " +
												std::to_string(newRow.size()) + 
												" to a matrix with the row length of " +
												std::to_string(colsCount_) + ".");
		}

		for (std::size_t i = 0; i < newRow.size(); ++i) {
			data_.push_back(newRow[i]);
		}
	}

	void AddRow_(const std::vector<std::string>& newRow) {
		rowsCount_++;

		if (newRow.size() != colsCount_) {
			throw WrongMatrixDimensionException("You are trying to add a row with the length of " +
												std::to_string(newRow.size()) + 
												" to a matrix with the row length of " +
												std::to_string(colsCount_) + ".");
		}

		for (std::size_t i = 0; i < newRow.size(); ++i) {
			// Convert string to a numeric type
			std::istringstream ss_(newRow[i]);
    		T num_;
    		ss_ >> num_;

			data_.push_back(num_);
		}
	}

	std::vector<std::string> TokenizeString_(const std::string& str, const char& delimiter) {
		std::vector<std::string> tokens_;

		std::stringstream ss_;
		ss_.str(str);
		std::string item_;
		while (std::getline(ss_, item_, delimiter)) {
			std::string token(item_);
			tokens_.push_back(item_);
		}

		return tokens_;
	}

/* Constructors & Destructor */
public:
	Matrix() { ResizeMatrix_(0, 0); }
	Matrix(const std::size_t& rows, const std::size_t& cols) { ResizeMatrix_(rows, cols); }
	Matrix(const Matrix<T>& m) : 
		rowsCount_(m.rowsCount_), 
		colsCount_(m.colsCount_) { data_ = m.data_; }		

	Matrix(const std::string& fileName, const char& delimiter) {
		std::ifstream file_(fileName);
		std::string line_;

		// Read the first line to find out how many columns are in matrix
		std::string firstLine_;
		std::getline(file_, firstLine_);
		
		auto tokens_ = TokenizeString_(firstLine_, delimiter);
		colsCount_ = tokens_.size();
		rowsCount_ = 0;
		AddRow_(tokens_);

		while (std::getline(file_, line_)) {
			tokens_ = TokenizeString_(line_, delimiter);
			AddRow_(tokens_);
		}
    }

	~Matrix() { }

/* General Operators */
public:
	bool operator== (const Matrix<T>& m) const {
		return (this->rowsCount_ == m.rowsCount_ && this->colsCount_ == m.colsCount_);
	}

	bool operator!= (const Matrix<T>& m) const {
		return !(*this == m);
	}

	Matrix<T>& operator+= (const Matrix<T>& m) {
		if (*this != m)
			throw WrongMatrixDimensionException("Both matrices must be of same shape.");

		for (std::size_t row = 0; row < this->rowsCount_; ++row) {
			for (std::size_t column = 0; column < this->colsCount_; ++column) {
				(*this)(row, column) += m(row, column);
			}
		}
		return *this;
	}

	Matrix<T>& operator-= (const Matrix<T>& m) {
		if (*this != m)
			throw WrongMatrixDimensionException("Both matrices must be of same shape.");

		for (std::size_t row = 0; row < this->rowsCount_; ++row) {
			for (std::size_t column = 0; column < this->colsCount_; ++column) {
				(*this)(row, column) -= m(row, column);
			}
		}
		return *this;
	}

	Matrix<T>& operator+= (const T& scalar) {		
		for (std::size_t row = 0; row < this->rowsCount_; ++row) {
			for (std::size_t column = 0; column < this->colsCount_; ++column) {
				(*this)(row, column) += scalar;
			}
		}
		return *this;
	}

	Matrix<T>& operator-= (const T& scalar) {					
		for (std::size_t row = 0; row < this->rowsCount_; ++row) {
			for (std::size_t column = 0; column < this->colsCount_; ++column) {
				(*this)(row, column) -= scalar;
			}
		}
		return *this;
	}

	T& operator() (const std::size_t& rowPos, const std::size_t& colPos) {			
		if (rowPos > rowsCount_ || colPos > colsCount_)
			throw InvalidIndexException("Index you are trying to access is not valid.");

		return data_[rowPos * colsCount_ + colPos];
	}

	const T& operator() (const std::size_t& rowPos, const std::size_t& colPos) const {			
		if (rowPos > rowsCount_ || colPos > colsCount_)
			throw InvalidIndexException("Index you are trying to access is not valid.");

		return data_[colsCount_ * rowPos + colPos];
	}

	const Matrix<T>& operator= (const Matrix<T>& m) {
		//ResizeMatrix_(m.rowsCount_, m.colsCount_);

		//if (this->rowsCount_ != m.rowsCount_ || this->colsCount_ != m.colsCount_)
		/*if (*this != m)
			throw WrongMatrixDimensionException("You are trying to assign to a matrix with different shape.");
		*/
		rowsCount_ = m.rowsCount_;
		colsCount_ = m.colsCount_;
		data_ = m.data_;				

		return *this;
		/*
		if (this->rowsCount_ == m.rowsCount_ && this->colsCount_ == m.colsCount_) {
			this->data_ = m.data_;
			this->rowsCount_ = m.rowsCount_;
			this->colsCount_ = m.colsCount_;
			return *this;
		}
		else {
			throw WrongMatrixDimensionException("Both matrices must have same dimensions.");
		}
		*/
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
			throw WrongMatrixDimensionException("Both matrices must be of same shape.");
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
				//T newValue_ = 0;
				for (std::size_t inner = 0; inner < this->colsCount_; ++inner) {
					//newValue += (*this)(row, inner) * m(inner, column);
					newMatrix_(row, column) += (*this)(row, inner) * m(inner, column);
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
				//T newValue_ = 0;
				for (std::size_t inner = 0; inner < this->colsCount_; ++inner) {
					//newValue += (*this)(row, inner) * m(inner, column);
					newMatrix_(row, column) += (*this)(row, inner) * m(inner, column);
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
	
	typename std::vector<T>::const_iterator GetDataCBegin() const { return data_.cbegin(); }
	typename std::vector<T>::const_iterator GetDataCEnd() const { return data_.cend(); }

	typename std::vector<T>::iterator GetDataBegin() { return data_.begin(); }
	typename std::vector<T>::iterator GetDataEnd() { return data_.end(); }

	Matrix<T> GetRow(std::size_t rowIndex) {
		if (rowIndex > rowsCount_ - 1 || rowIndex < 0) {
			throw InvalidIndexException("You are trying to access row at index " + 
										std::to_string(rowIndex) + 
										". Matrix has only " + 
										std::to_string(rowsCount_) +
										" rows.");
		}

		Matrix<T> newMatrix_(1, colsCount_);
		for (std::size_t row = 0; row < rowsCount_; ++row) {
			if (row == rowIndex) {
				for (std::size_t column = 0; column < colsCount_; ++column) {
					newMatrix_(0, column) = (*this)(row, column);
				}
				break;			
			}
		}

		return newMatrix_;
	}

	const Matrix<T> GetRow(std::size_t rowIndex) const {
		if (rowIndex > rowsCount_ - 1 || rowIndex < 0) {
			throw InvalidIndexException("You are trying to access row at index " + 
										std::to_string(rowIndex) + 
										". Matrix has only " + 
										std::to_string(rowsCount_) +
										" rows.");
		}

		Matrix<T> newMatrix_(1, colsCount_);
		for (std::size_t row = 0; row < rowsCount_; ++row) {
			if (row == rowIndex) {
				for (std::size_t column = 0; column < colsCount_; ++column) {
					newMatrix_(0, column) = (*this)(row, column);
				}
				break;
			}			
		}

		return newMatrix_;
	}

	Matrix<T> GetColumn(std::size_t columnIndex) {
		if (columnIndex > colsCount_ - 1 || columnIndex < 0) {
			throw InvalidIndexException("You are trying to access column at index " + 
										std::to_string(columnIndex) + 
										". Matrix has only " + 
										std::to_string(colsCount_) +
										" columns.");
		}

		Matrix<T> newMatrix_(rowsCount_, 1);
        for (std::size_t column = 0; column < colsCount_; ++column) {
            if (column == columnIndex) {
                for (std::size_t row = 0; row < rowsCount_; ++row) {
					newMatrix_(row, 0) = (*this)(row, column);
				}
				break;
			}
		}

		return newMatrix_;
	}

	const Matrix<T> GetColumn(std::size_t columnIndex) const {
		if (columnIndex > colsCount_ - 1 || columnIndex < 0) {
			throw InvalidIndexException("You are trying to access column at index " + 
										std::to_string(columnIndex) + 
										". Matrix has only " + 
										std::to_string(colsCount_) +
										" columns.");
		}

		Matrix<T> newMatrix_(rowsCount_, 1);
		for (std::size_t column = 0; column < colsCount_; ++column) {
			if (column == columnIndex) {
				for (std::size_t row = 0; row < rowsCount_; ++row) {
					newMatrix_(row, 0) = (*this)(row, column);
				}
				break;
			}
		}

		return newMatrix_;
	}

	void SetRow(std::size_t rowIndex, const Matrix<T>& newRow) {
		if (rowIndex > rowsCount_ - 1 || rowIndex < 0) {
			throw InvalidIndexException("You are trying to set row at index " + 
										std::to_string(rowIndex) + 
										". Matrix has only " + 
										std::to_string(rowsCount_) +
										" rows.");
		}

		// newRow should be a matrix of a shape (1, this->rowsCount_)
		if (newRow.rowsCount_ != 1) {
			throw WrongMatrixDimensionException("You are trying to assign multiple or zero rows to a single row.");
		}

		if (newRow.colsCount_ != colsCount_) {
			throw WrongMatrixDimensionException("You are trying to assign a row with the length of " +
												std::to_string(newRow.colsCount_) + 
												" to a matrix with the row length of " +
												std::to_string(colsCount_) + ".");
		}

		for (std::size_t i = 0; i < newRow.colsCount_; ++i) {
			data_[rowIndex * colsCount_ + i] = newRow(0, i);
		}
	}

	void SetRow(std::size_t rowIndex, const std::vector<T>& newRow) const {
		if (rowIndex > rowsCount_ - 1 || rowIndex < 0) {
			throw InvalidIndexException("You are trying to set row at index " + 
										std::to_string(rowIndex) + 
										". Matrix has only " + 
										std::to_string(rowsCount_) +
										" rows.");
		}		

		if (newRow.size() != colsCount_) {
			throw WrongMatrixDimensionException("You are trying to assign a row with the length of " +
												std::to_string(newRow.size()) + 
												" to a matrix with the row length of " +
												std::to_string(colsCount_) + ".");
		}

		for (std::size_t i = 0; i < newRow.size(); ++i) {
			data_[rowIndex * colsCount_ + i] = newRow[i];
		}
	}

	void SetColumn(std::size_t columnIndex, const Matrix<T>& newColumn) {
		if (columnIndex > colsCount_ - 1 || columnIndex < 0) {
			throw InvalidIndexException("You are trying to set column at index " + 
										std::to_string(columnIndex) + 
										". Matrix has only " + 
										std::to_string(colsCount_) +
										" columns.");
		}

		// newColumn should be a matrix of a shape (this->colsCount_, 1)
		if (newColumn.colsCount_ != 1) {
			throw WrongMatrixDimensionException("You are trying to assign multiple or zero columns to a single column.");
		}

		if (newColumn.rowsCount_ != rowsCount_) {
			throw WrongMatrixDimensionException("You are trying to assign a column with the length of " +
												std::to_string(newColumn.rowsCount_) + 
												" to a matrix with the column length of " +
												std::to_string(rowsCount_) + ".");
		}

		for (std::size_t i = 0; i < newColumn.rowsCount_; ++i) {			
			data_[i * colsCount_ + columnIndex] = newColumn(i, 0); 
		}
	}
	
	void SetColumn(std::size_t columnIndex, const std::vector<T>& newColumn) {
		if (columnIndex > colsCount_ - 1 || columnIndex < 0) {
			throw InvalidIndexException("You are trying to set column at index " + 
										std::to_string(columnIndex) + 
										". Matrix has only " + 
										std::to_string(colsCount_) +
										" columns.");
		}		

		if (newColumn.size() != rowsCount_) {
			throw WrongMatrixDimensionException("You are trying to assign a column with the length of " +
												std::to_string(newColumn.size()) + 
												" to a matrix with the column length of " +
												std::to_string(rowsCount_) + ".");
		}

		for (std::size_t i = 0; i < newColumn.size(); ++i) {
			data_[i * colsCount_ + columnIndex] = newColumn[i];
		}
	}

	Matrix<T> GetRowsFromIndex(std::size_t startIndex, std::size_t totalRows) {
		Matrix<T> m(totalRows, this->colsCount_);
		
		std::size_t i = 0;
		for (std::size_t rowIndex = startIndex; rowIndex < startIndex + totalRows; ++rowIndex) {			
			m.SetRow(i, this->GetRow(rowIndex));
			i++;			
		}
		return m;
	}

	const Matrix<T> GetRowsFromIndex(std::size_t startIndex, std::size_t totalRows) const {
		Matrix<T> m(totalRows, this->colsCount_);
		
		std::size_t i = 0;
        for (std::size_t rowIndex = startIndex; rowIndex < startIndex + totalRows; ++rowIndex) {
			m.SetRow(i, this->GetRow(rowIndex));
			i++;
		}
		return m;
	}

/* Public Methods */
public:	
	void ReshapeWithMatrix(const Matrix<T>& m) {
		ResizeMatrix_(m.rowsCount_, m.colsCount_);
		this->data_ = m.data_;
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

	void RandomInitialization(const std::size_t& rows, const std::size_t& cols) {
		ResizeMatrix_(rows, cols);

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

	void InitializeWithZeros() {
		std::for_each(this->data_.begin(), this->data_.end(), [&](T& el) {
			el = 0;
		});
	}

	void InitializeWithZeros(const std::size_t& rows, const std::size_t& cols) {
		ResizeMatrix_(rows, cols);

		std::for_each(this->data_.begin(), this->data_.end(), [&](T& el) {
			el = 0;
		});
	}

	void InitializeWithOnes() {
		std::for_each(this->data_.begin(), this->data_.end(), [&](T& el) {
			el = 1;
		});
	}

	void InitializeWithOnes(const std::size_t& rows, const std::size_t& cols) {
		ResizeMatrix_(rows, cols);

		std::for_each(this->data_.begin(), this->data_.end(), [&](T& el) {
			el = 1;
		});
	}

	Matrix<T> Transpose() {
		Matrix<T> transposed_(colsCount_, rowsCount_);

		for (std::size_t row = 0; row < rowsCount_; ++row) {
			for (std::size_t col = 0; col < colsCount_; ++col) {
				transposed_(col, row) = (*this)(row, col);
			}
		}

		return transposed_;
	}

	const Matrix<T> Transpose() const {
		Matrix<T> transposed_(colsCount_, rowsCount_);

		for (std::size_t row = 0; row < rowsCount_; ++row) {
			for (std::size_t col = 0; col < colsCount_; ++col) {
				transposed_(col, row) = (*this)(row, col);
			}
		}

		return transposed_;
	}
};
