#include <iostream>
#include <vector>
#include <typeinfo>
#include <memory>

#include "Matrix.h"
#include "Vector.h"
#include "Graph.h"
#include "Layers/ReluLayer.h"
#include "Layers/SigmoidLayer.h"
#include "Layers/IdentityLayer.h"
#include "Functions/LossFunctions/MSELossFunction.h"

typedef Functions::MSELossFunction<double> MSEDouble;

int main() {		
	// A row represents one input vector of length threeą
	Matrix<double> m1(1, 3); // 1x3
	m1.RandomInitialization();

	Matrix<double> expected(1, 1);
	expected.RandomInitialization();

	std::cout << "Expected: " << expected << std::endl;
	
	Graph<double, MSEDouble> g(m1, expected);
	g.AddLayer<Layers::IdentityLayer<double>>(1); // 1x1
	const auto& graphOutput = g.Run(); // 1x1

	std::cout << "Run:" << std::endl;
	std::cout << graphOutput << std::endl;
	std::cout << "==========" << std::endl;

	std::cout << "Done" << std::endl;
	return 0;
}
