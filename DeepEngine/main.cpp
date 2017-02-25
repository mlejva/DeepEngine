#include <iostream>
#include "Matrix.h"
#include "ReluActivationFunction.h"
#include "SigmoidActivationFunction.h"
#include "TanhActivationFunction.h"
#include "MSELossFunction.h"

int main(int argc, char* argv[]) {
	Matrix<float> m1(3, 2);
	Matrix<float> m2(2, 2);
	
	m1.SetElement(1, 0, -5);
	m1.SetElement(1, 1, -2);
	m2.SetElement(0, 1, 5);

	auto m3 = m1 + 4;

	//std::cout << m1 << std::endl;
	//std::cout << m2 << std::endl;
	std::cout << m3 << std::endl;
	m3.ApplyFunctionElementWise<TanhActivationFunction<float>>();
	std::cout << m3 << std::endl;

	Matrix<int> y1(3, 1);
	Matrix<int> y2(3, 1);
	

	system("pause");
	return 0;
}

