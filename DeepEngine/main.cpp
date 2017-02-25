#include <iostream>
#include "Matrix.h"

int main(int argc, char* argv[]) {
	Matrix<float> m1(3, 2);
	Matrix<float> m2(2, 2);
	
	m1.SetElement(1, 0, 5.5);
	m2.SetElement(0, 1, 5.5);

	auto m3 = m1 - 5;

	std::cout << m1 << std::endl;
	std::cout << m2 << std::endl;
	std::cout << m3 << std::endl;



	system("pause");
	return 0;
}

