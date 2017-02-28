#include <iostream>
#include "Matrix.h"
#include "ReluActivationFunction.h"
#include "SigmoidActivationFunction.h"
#include "TanhActivationFunction.h"
#include "MSELossFunction.h"

int main(int argc, char* argv[]) {
	Matrix<float> m1(3, 2);
	Matrix<float> m2(3, 2);
	
	m1.SetElement(1, 0, -5);
	m1.SetElement(1, 1, -2);
	m2.SetElement(0, 1, 5);
	
	try {
		auto m3 = m1 * m2;
	}
	catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}

	//std::cout << m1 << std::endl;
	//std::cout << m2 << std::endl;
	/*std::cout << m3 << std::endl;
	m3.ApplyFunctionElementWise<TanhActivationFunction<float>>();
	std::cout << m3 << std::endl;*/

	Matrix<float> y1(3, 1);
	Matrix<float> y2(3, 1);
	y1.RandomInitialization();
	y2.RandomInitialization();
	std::cout << y1 << std::endl;
	std::cout << y2 << std::endl;


	MSELossFunction<float> mse;
	std::cout << mse.apply(y1, y2) << std::endl;

	system("pause");
	return 0;
}

