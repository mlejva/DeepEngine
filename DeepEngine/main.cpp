#include <iostream>
#include <vector>
#include <typeinfo>
#include "Matrix.h"
#include "ReluActivationFunction.h"
#include "SigmoidActivationFunction.h"
#include "TanhActivationFunction.h"
#include "MSELossFunction.h"
#include "Vector.h"
#include "Graph.h"

int main() {	
	/*Matrix<float> m1(3, 3);
	Matrix<float> m2(3, 2);
	
	m1.SetElement(1, 0, -5);
	m1.SetElement(1, 1, -2);
	m2.SetElement(0, 1, 5); 

	Graph<float> g; */
	
	/*g.SetInput(m1);
	Graph<float>::TanhLayer tl;
	g.AddLayer(tl);*/
	//auto output = g.Test();

	Matrix<double> m1(4, 4);
	Matrix<double> m2(4, 4);
	m1.RandomInitialization();
	m2.RandomInitialization();
	std::cout << m1 << std::endl;
	std::cout << m2 << std::endl;

	auto m3 = m1 + m2;
	std::cout << m3 << std::endl;
	

	//std::cin.get();

	return 0;
}
