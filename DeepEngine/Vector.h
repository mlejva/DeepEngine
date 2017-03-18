#pragma once

#include "Matrix.h"

template <typename T>
class Vector : public Matrix<T> {

public:
    Vector() : Matrix<T>(0, 0) { }
    Vector(std::size_t height) : Matrix<T>(height, 1) { }

public:
    void InitializeWithZeros(const std::size_t& rows) {
		Matrix<T>::InitializeWithZeros(rows, 1);		
	}
};