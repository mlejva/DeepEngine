#pragma once

#include <iostream>
#include <typeinfo>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <random>
#include <string>
#include <vector>
#include <exception>

class MatrixShapeException : public std::runtime_error {
public:
	MatrixShapeException(const std::string& message) : runtime_error(message) { }
};

class IndexException : public std::runtime_error {
public:
	IndexException(const std::string& message) : runtime_error(message) { }
};

template <typename T>
class Matrix {
/* Public Static Methods */
public:
	/*// Computes a multiplication between two matrices
	static Matrix<T> dot(const Matrix<T>& left, const Matrix<T>& right) {

	}*/
	
	// Computes a Hadamard product (i.e. element-wise product) of two matrices
	static Matrix<T> Multiply(const Matrix<T>& left, const Matrix<T>& right) {
		if (left != right)
			throw MatrixShapeException("Both matrices must have the same shape.");
		
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
		
		// Add first line
		auto tokens_ = TokenizeString_(firstLine_, delimiter);
		colsCount_ = tokens_.size();
		rowsCount_ = 0;
		AddRow_(tokens_);

		// Add every other line
		while (std::getline(file_, line_)) {
			tokens_ = TokenizeString_(line_, delimiter);
			AddRow_(tokens_);
		}
    }

	~Matrix() { }

/* Private Methods */
private: 
	void ResizeMatrix_(const std::size_t& rows, const std::size_t& cols) { 
		rowsCount_ = rows;
		colsCount_ = cols;

		data_ = std::vector<T>(rowsCount_ * colsCount_);
	}

	void AddRow_(const std::vector<T>& newRow) {
		rowsCount_++;

		if (newRow.size() != colsCount_) {
			throw MatrixShapeException("You are trying to add a row with the length of " +
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
			throw MatrixShapeException("You are trying to add a row with the length of " +
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



/* General Operators */
public:
	T& operator() (const std::size_t& rowPos, const std::size_t& colPos) {			
		if (rowPos > (rowsCount_ - 1)|| colPos > (colsCount_ - 1))
			throw IndexException("Index you are trying to access is not valid.");

		return data_[rowPos * colsCount_ + colPos];
	}

	const T& operator() (const std::size_t& rowPos, const std::size_t& colPos) const {			
		if (rowPos > (rowsCount_ - 1) || colPos > (colsCount_ - 1))
			throw IndexException("Index you are trying to access is not valid.");

		return data_[colsCount_ * rowPos + colPos];
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
	const Matrix<T>& operator= (const Matrix<T>& m) {	
		// We can assign new matrix to an uninitialized matrix (i.e. a matrix with a shape (0, 0))
		if ((this->rowsCount_ == m.rowsCount_ && this->colsCount_ == m.colsCount_) || 
			(this->rowsCount_ == 0 && this->colsCount_ == 0)) {
			
			rowsCount_ = m.rowsCount_;
			colsCount_ = m.colsCount_;
			data_ = m.data_;				
		}
		else {
			throw MatrixShapeException("You are trying to assign to a matrix with different shape.");
		}

		return *this;		
	}

	bool operator== (const Matrix<T>& m) const {
		return (this->rowsCount_ == m.rowsCount_ && this->colsCount_ == m.colsCount_);
	}

	bool operator!= (const Matrix<T>& m) const {
		return !(*this == m);
	}

	Matrix<T>& operator+= (const Matrix<T>& m) {
		if (*this != m)
			throw MatrixShapeException("Both matrices must be of same shape.");

		for (std::size_t row = 0; row < this->rowsCount_; ++row) {
			for (std::size_t column = 0; column < this->colsCount_; ++column) {
				(*this)(row, column) += m(row, column);
			}
		}
		return *this;
	}

	Matrix<T>& operator-= (const Matrix<T>& m) {
		if (*this != m)
			throw MatrixShapeException("Both matrices must be of same shape.");

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
			throw MatrixShapeException("Both matrices must be of same shape.");
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
			throw MatrixShapeException("Both matrices must have same dimensions.");
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
			throw MatrixShapeException("Both matrices must have same dimensions.");
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
			throw MatrixShapeException("Both matrices must have same dimensions.");
		}
	}

	Matrix<T> operator* (const Matrix<T>& m) {
		if (this->colsCount_ != m.rowsCount_)				
			throw MatrixShapeException("The left-hand matrix must have the same width as is the height of the right-hand matrix.");

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
			throw MatrixShapeException("The left-hand matrix must have the same width as is the height of the right-hand matrix.");			

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

	Matrix<T> GetRow(const std::size_t& rowIndex) {
		if (rowIndex > (rowsCount_ - 1) || rowIndex < 0) {
			throw IndexException("You are trying to access row at index " + 
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

	const Matrix<T> GetRow(const std::size_t& rowIndex) const {
		if (rowIndex > (rowsCount_ - 1) || rowIndex < 0) {
			throw IndexException("You are trying to access row at index " + 
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

	Matrix<T> GetColumn(const std::size_t& columnIndex) {
		if (columnIndex > (colsCount_ - 1) || columnIndex < 0) {
			throw IndexException("You are trying to access column at index " + 
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

	const Matrix<T> GetColumn(const std::size_t& columnIndex) const {
		if (columnIndex > (colsCount_ - 1) || columnIndex < 0) {
			throw IndexException("You are trying to access column at index " + 
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

	void SetRow(const std::size_t& rowIndex, const Matrix<T>& newRow) {
		if (rowIndex > (rowsCount_ - 1) || rowIndex < 0) {
			throw IndexException("You are trying to set row at index " + 
										std::to_string(rowIndex) + 
										". Matrix has only " + 
										std::to_string(rowsCount_) +
										" rows.");
		}

		// newRow should be a matrix of a shape (1, this->rowsCount_)
		if (newRow.rowsCount_ != 1) {
			throw MatrixShapeException("You are trying to assign multiple or zero rows to a single row.");
		}

		if (newRow.colsCount_ != colsCount_) {
			throw MatrixShapeException("You are trying to assign a row with the length of " +
												std::to_string(newRow.colsCount_) + 
												" to a matrix with the row length of " +
												std::to_string(colsCount_) + ".");
		}

		for (std::size_t i = 0; i < newRow.colsCount_; ++i) {
			data_[rowIndex * colsCount_ + i] = newRow(0, i);
		}
	}

	void SetRow(const std::size_t& rowIndex, const std::vector<T>& newRow) const {
		if (rowIndex > (rowsCount_ - 1) || rowIndex < 0) {
			throw IndexException("You are trying to set row at index " + 
										std::to_string(rowIndex) + 
										". Matrix has only " + 
										std::to_string(rowsCount_) +
										" rows.");
		}		

		if (newRow.size() != colsCount_) {
			throw MatrixShapeException("You are trying to assign a row with the length of " +
												std::to_string(newRow.size()) + 
												" to a matrix with the row length of " +
												std::to_string(colsCount_) + ".");
		}

		for (std::size_t i = 0; i < newRow.size(); ++i) {
			data_[rowIndex * colsCount_ + i] = newRow[i];
		}
	}

	void SetColumn(const std::size_t& columnIndex, const Matrix<T>& newColumn) {
		if (columnIndex > (colsCount_ - 1) || columnIndex < 0) {
			throw IndexException("You are trying to set column at index " + 
										std::to_string(columnIndex) + 
										". Matrix has only " + 
										std::to_string(colsCount_) +
										" columns.");
		}

		// newColumn should be a matrix of a shape (this->colsCount_, 1)
		if (newColumn.colsCount_ != 1) {
			throw MatrixShapeException("You are trying to assign multiple or zero columns to a single column.");
		}

		if (newColumn.rowsCount_ != rowsCount_) {
			throw MatrixShapeException("You are trying to assign a column with the length of " +
												std::to_string(newColumn.rowsCount_) + 
												" to a matrix with the column length of " +
												std::to_string(rowsCount_) + ".");
		}

		for (std::size_t i = 0; i < newColumn.rowsCount_; ++i) {			
			data_[i * colsCount_ + columnIndex] = newColumn(i, 0); 
		}
	}
	
	void SetColumn(const std::size_t& columnIndex, const std::vector<T>& newColumn) {
		if (columnIndex > (colsCount_ - 1) || columnIndex < 0) {
			throw IndexException("You are trying to set column at index " + 
										std::to_string(columnIndex) + 
										". Matrix has only " + 
										std::to_string(colsCount_) +
										" columns.");
		}		

		if (newColumn.size() != rowsCount_) {
			throw MatrixShapeException("You are trying to assign a column with the length of " +
												std::to_string(newColumn.size()) + 
												" to a matrix with the column length of " +
												std::to_string(rowsCount_) + ".");
		}

		for (std::size_t i = 0; i < newColumn.size(); ++i) {
			data_[i * colsCount_ + columnIndex] = newColumn[i];
		}
	}

	
	Matrix<T> GetRowsFromIndex(const std::size_t& startIndex, const std::size_t& totalRows) {
		if (startIndex > (rowsCount_ - 1) || startIndex < 0) {
			throw IndexException("You are trying to get rows starting from index " + 
									std::to_string(startIndex) + 
								 	". Matrix has only " + 
								 	std::to_string(rowsCount_) +
								 	" rows.");
		}
		Matrix<T> m(totalRows, this->colsCount_);

		// If endIndex is greater than (startIndex + totalRows) return all possible rows
		std::size_t endIndex_ = startIndex + totalRows;
		if (endIndex_ > (rowsCount_ - 1))
			endIndex_ = (rowsCount_ - 1);

		std::size_t i = 0;
		for (std::size_t rowIndex = startIndex; rowIndex < endIndex_; ++rowIndex) {			
			m.SetRow(i, this->GetRow(rowIndex));
			i++;			
		}
		return m;
	}

	const Matrix<T> GetRowsFromIndex(const std::size_t& startIndex, const std::size_t& totalRows) const {
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

	void XavierInitialization(const std::size_t& rows,
							  const std::size_t& cols,
							  const std::size_t& layerInputSize,
							  const std::size_t& layerOutputSize) {
		ResizeMatrix_(rows, cols);

		std::random_device rd_; // Obtain a random number from hardware
		// A Mersenne Twister pseudo-random generator of 32-bit numbers 
		// with a state size of 19937 bits.
		std::mt19937 gen_(rd_()); // Seed the generator
		
		if (typeid(T) == typeid(int)) {
			// Xavier initialization
			int start_ = static_cast<int>( -sqrt(6 / (layerInputSize + layerOutputSize)) );
			int end_ = static_cast<int>( sqrt(6 / (layerInputSize + layerOutputSize)) );
			std::uniform_int_distribution<> distr_(start_, end_);

			std::for_each(this->data_.begin(), this->data_.end(), [&](T& el_) {
				el_ = distr_(gen_);
			});
		}
		else {
			// Xavier initialization
			T start_ = -sqrt(6.0 / (layerInputSize + layerOutputSize));
			T end_ = sqrt(6.0 / (layerInputSize + layerOutputSize));
			std::uniform_real_distribution<T> distr_(start_, end_);

			std::for_each(this->data_.begin(), this->data_.end(), [&](T& el_) {
				el_ = distr_(gen_);
			});
		}	
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

	void SaveToFile(const std::string& path) const {
		std::ofstream outputFile_(path);		
		outputFile_ << *this;
	}
};
