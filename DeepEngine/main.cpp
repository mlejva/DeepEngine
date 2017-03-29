
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
	Matrix<double> input(1, 2); // 1x2
	input(0, 0) = 0.05;
	input(0, 1) = 0.10;
	std::cout << "Network Input:\n" << input << "\n=======" << std::endl;

	Matrix<double> expected(1, 2);
	expected(0, 0) = 0.01;
	expected(0, 1) = 0.99;
	std::cout << "Network Expected:\n" << expected << "\n=======" << std::endl;
	
	Graph<double, MSEDouble> g(input, expected);
	g.AddLayer<Layers::SigmoidLayer<double>>(2); // 1x2
	g.AddLayer<Layers::SigmoidLayer<double>>(2); // 1x2
	const auto& graphOutput = g.Run(); // 1x2

	std::cout << "Run:" << std::endl;
	std::cout << graphOutput << std::endl;
	std::cout << "==========" << std::endl;

	std::cout << "Done" << std::endl;
	return 0;
}
