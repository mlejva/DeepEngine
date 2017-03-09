#pragma once

#include "Matrix.h"
#include "Functions/ActivationFunctions/ActivationFunctionInterface.h"

template <typename T>
class LayerInterface {

/* Children Properties */
protected:
    Matrix<T> input_;
    ActivationFunctionInterface<T> activationFunction_;

/* Children Methods */
protected:
    virtual void ApplyActivationFunction() = 0;

/* Constructors & Destructor */
public:
    LayerInterface() { }
	virtual ~LayerInterface() { }

/* Public Methods */
public:
    void SetInput(const Matrix<T>& input) {
        input_ = input;
    }

public:
    Matrix<T> output;
};
