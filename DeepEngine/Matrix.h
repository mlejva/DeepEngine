#pragma once

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <random>
#include <cmath>
#include "FunctionInterface.h"

template <class T>
class Matrix {

	////////////////// PUBLIC TYPEDEFS //////////////////
public:
	typedef T value_type;
	typedef std::size_t size_type;
	//typedef typename T::size_type size_type;
	//typedef T::size_type size_type;

	////////////////// PRIVATE TYPEDEFS //////////////////
private:
	typedef T* pointer_type;
	typedef T** storage_type;

	////////////////// PRIVATE VARIABLES //////////////////
private:
	size_type width_;
	size_type height_;
	storage_type data_;

	////////////////// PRIVATE ITERATOR CLASSES //////////////////
private:
	template <typename P, typename V> // "P" - pointer type, "V" - value type
	class Iterator : public std::iterator<std::forward_iterator_tag, T> {

	protected:
		P itData_;
		size_type w_; // width of the matrix
		size_type h_; // heigth of the matrix

	public:
		Iterator(P d, size_type width, size_type height) : itData_(d), w_(width), h_(height) { }
		Iterator() : itData_(nullptr), w_(0), h_(0) { }

		V& operator*() const {
			return *itData_;
		}

		Iterator<P, V>& operator++() {
			++itData_;
			return *this;
		}

		Iterator<P, V> operator++(int) {
			Iterator<P, V> tmp(*this);
			++itData_;
			return tmp
		}

		Iterator<P, V>& operator= (V value) {
			*itData_ = value;
			return *this;
		}

		const P operator->() {
			return itData_;
		}

		friend bool operator==(const Iterator& lhs, const Iterator& rhs) {
			return !(lhs.itData_ != rhs.itData_);
		}

		friend bool operator!=(const Iterator& lhs, const Iterator& rhs) {
			return !(lhs.itData_ == rhs.itData_);
		}
	};

	template <typename P, typename V> // "P" - pointer type, "V" - value type
	class V_Iterator : public Iterator<P, V> {
	public:
		V_Iterator(P d, size_type width, size_type height) : Iterator<P, V>(d, width, height) { }
		V_Iterator() : Iterator<P, V>(nullptr, 0, 0) { }

		V_Iterator<P, V>& operator++() {
			this->itData_ += this->w_;
			return *this;
		}

		V_Iterator<P, V> operator++(int) {
			V_Iterator<P, V> tmp(*this);
			this->itData_ += this->w_;
			return tmp;
		}
	};

	template <typename P, typename V> // "P" - pointer type, "V" - value type
	class H_Iterator : public Iterator<P, V> {
	public:
		H_Iterator(P d, size_type width, size_type height) : Iterator<P, V>(d, width, height) { }
		H_Iterator() : Iterator<P, V>(nullptr, 0, 0) { }
	};

	////////////////// PUBLIC ITERATOR TYPEDEFS //////////////////
public:
	typedef Iterator<T*, T> iterator;
	typedef Iterator<const T*, const T> const_iterator;

	typedef V_Iterator<T*, T> viterator;
	typedef V_Iterator<const T*, const T> const_viterator;

	typedef H_Iterator<T*, T> hiterator;
	typedef H_Iterator<const T*, const T> const_hiterator;

	////////////////// CONSTRUCTORS & DESTRUCTORS //////////////////
public:
	explicit Matrix(const std::string& file_name) {
		std::ifstream infile;
		infile.open(file_name);

		infile >> width_ >> height_;
		data_ = CreateMatrix_(height_, width_);

		for (Matrix<T>::iterator it = this->begin(); it != this->end(); ++it)
			infile >> *it;
	}

	Matrix(const size_type& height, const size_type& width) :
		width_(width),
		height_(height),
		data_(CreateMatrix_(height, width)) {

		// fill the created matrix with default values of "T"
		for (Matrix<T>::iterator it = this->begin(); it != this->end(); ++it)
			*it = T();
	}

	// copy constructor
	Matrix(const Matrix<T>& m) {
		width_ = m.width_;
		height_ = m.height_;

		data_ = CreateMatrix_(height_, width_);
#pragma warning(push)
#pragma warning(disable:4996)
		std::copy(&m.data_[0][0], &m.data_[0][0] + width_ * height_, &data_[0][0]);
#pragma warning(pop)
	}

	~Matrix() {
		delete[] data_[0]; // because in data_[0] is array of value_type
		delete[] data_;
	}

	////////////////// PUBLIC METHODS //////////////////
public:
	const_iterator begin() const { return Iterator<const T*, const T>(*data_, width_, height_); }
	const_iterator end() const { return Iterator<const T*, const T>(*data_ + height_ * width_, width_, height_); }

	iterator begin() { return Iterator<T*, T>(*data_, width_, height_); }
	iterator end() { return Iterator<T*, T>(*data_ + height_ * width_, width_, height_); }

	const_viterator v_begin(size_type column) const { return V_Iterator<const T*, const T>(*data_ + column, width_, height_); }
	const_viterator v_end(size_type column) const { return V_Iterator<const T*, const T>(*data_ + column + height_ * width_, width_, height_); }

	viterator v_begin(size_type column) { return V_Iterator<T*, T>(*data_ + column, width_, height_); }
	viterator v_end(size_type column) { return V_Iterator<T*, T>(*data_ + column + height_ * width_, width_, height_); }

	const_hiterator h_begin(size_type row) const { return H_Iterator<const T*, const T>(*data_ + width_ * row, width_, height_); }
	const_hiterator h_end(size_type row) const { return H_Iterator<const T*, const T>(*data_ + width_ * row + width_, width_, height_); }

	hiterator h_begin(size_type row) { return H_Iterator<T*, T>(*data_ + width_ * row, width_, height_); }
	hiterator h_end(size_type row) { return H_Iterator<T*, T>(*data_ + width_ * row + width_, width_, height_); }

	T& GetElement(size_type x, size_type y) const { return data_[x][y]; };
	void SetElement(size_type x, size_type y, const T value) { data_[x][y] = value; };


	size_type getWidth() const { return width_; }
	size_type getHeight() const { return height_; }

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
		T start_ = -(sqrt(6 / (width_ + height_)));
		T end_ = (sqrt(6 / (width_ + height_)));
		// TODO: only normal distribution for real numbers
		std::uniform_real_distribution<> distr_(start_, end_); // Define the range

		std::for_each(this->begin(), this->end(), [&](T& el_) {
			el_ = distr_(gen_);
		});
	}
	////////////////// PRIVATE METHODS //////////////////
private:
	storage_type CreateMatrix_(const size_type height, const size_type width) {
		storage_type d = new pointer_type[height]; // array with pointers pointing to rows inside the "block"
		pointer_type block = new value_type[width * height]; // one block of memory to store the data

		for (size_type row = 0; row < height; ++row)
			d[row] = &block[row * width];

		return d;
	}

	////////////////// PUBLIC MATRIX OPERATORS OVERRIDES //////////////////
public:
	friend std::ostream& operator<<(std::ostream& s, const Matrix<T>& m) {
		for (size_type i = 0; i < m.getHeight(); ++i) {			
			for (size_type j = 0; j < m.getWidth(); ++j) {
				if (j != m.getWidth() - 1)
					s << m.GetElement(i, j) << "\t";
				else
					s << m.GetElement(i, j);
			}
			s << std::endl;
		}

		return s;
	}

	pointer_type operator[](size_type i) const { return data_[i]; }
	//T& operator[][](size_type x, size_type y) const { return data_[x][y]; }
	
	// TODO: Wrong size of matrices exception for operators bellow
	Matrix<T> operator*(const Matrix<T>& m) const {
		Matrix<T> newMatrix_(this->height_, m.getWidth());
		for (size_type row = 0; row < newMatrix_.getHeight(); ++row) {
			for (size_type column = 0; column < newMatrix_.getWidth(); ++column) {
				T newValue_ = 0;
				for (size_type inner = 0; inner < this->width_;  ++inner) {
					 newValue_ += this->GetElement(row, inner) * m.GetElement(inner, column);
				}
				newMatrix_.SetElement(row, column, newValue_);
			}
		}
		return newMatrix_;
	}

	// Multiplication applied element-wise
	Matrix<T> operator*(const T& v) const { 
		Matrix<T> newMatrix_(this->height_, this->width_);
		for (size_type i = 0; i < this->height_; ++i) {
			for (size_type j = 0; j < this->width_; ++j) {
				T newValue_ = GetElement(i, j) * v;
				newMatrix_.SetElement(i, j, newValue_);
			}
		}
		return newMatrix_;
	}

	Matrix<T> operator+(const Matrix<T>& m) const {		
		Matrix<T> newMatrix_(this->height_, this->width_);
		for (size_type i = 0; i < this->height_; ++i) {
			for (size_type j = 0; j < this->width_; ++j) {
				T newValue_ = GetElement(i, j) + m.GetElement(i, j);
				newMatrix_.SetElement(i, j, newValue_);
			}
		}		
		return newMatrix_;
	}

	// Summation applied element-wise
	Matrix<T> operator+(const T& v) const {
		Matrix<T> newMatrix_(this->height_, this->width_);
		for (size_type i = 0; i < this->height_; ++i) {
			for (size_type j = 0; j < this->width_; ++j) {
				T newValue_ = GetElement(i, j) + v;
				newMatrix_.SetElement(i, j, newValue_);
			}
		}
		return newMatrix_;
	}

	Matrix<T> operator-(const Matrix<T>& m) const {
		Matrix<T> newMatrix_(this->height_, this->width_);
		for (size_type i = 0; i < this->height_; ++i) {
			for (size_type j = 0; j < this->width_; ++j) {
				T newValue_ = GetElement(i, j) - m.GetElement(i, j);
				newMatrix_.SetElement(i, j, newValue_);
			}
		}
		return newMatrix_;
	}

	// Subtraction applied element-wise
	Matrix<T> operator-(const T& v) const { 
		Matrix<T> newMatrix_(this->height_, this->width_);
		for (size_type i = 0; i < this->height_; ++i) {
			for (size_type j = 0; j < this->width_; ++j) {
				T newValue_ = GetElement(i, j) - v;
				newMatrix_.SetElement(i, j, newValue_);
			}
		}
		return newMatrix_;
	}
};