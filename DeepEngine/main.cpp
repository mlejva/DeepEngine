
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
	Matrix<double> input(4, 2);
	input(0, 0) = 0.0;
	input(0, 1) = 0.0;

	input(1, 0) = 0.0;
	input(1, 1) = 1.0;

	input(2, 0) = 1.0;
	input(2, 1) = 0.0;

	input(3, 0) = 1.0;
	input(3, 1) = 1.0;
	//std::cout << "Network Input:\n" << input << "\n=======" << std::endl;

	Matrix<double> expected(4, 1);
	expected(0, 0) = 0.0;
	expected(1, 0) = 1.0;
	expected(2, 0) = 1.0;
	expected(3, 0) = 0.0;
	

	//std::cout << "Network Expected:\n" << expected << "\n=======" << std::endl;
	
	Graph<double, MSEDouble> g;
	g.AddLayer<Layers::SigmoidLayer<double>>(3); // Hidden layer
	g.AddLayer<Layers::IdentityLayer<double>>(1); // Output layer
	
	Matrix<double> firstOutput_;
	Matrix<double> lastOutput_;
	for (auto i = 0; i < 1; ++i) {		
		const auto& graphOutput_ = g.Train(input, expected); // 1x2
		/*std::cout << "Run:" << std::endl;
		std::cout << graphOutput_ << std::endl;
		std::cout << "==========" << std::endl;
		*/

		lastOutput_ = graphOutput_;
		if (i == 0)
			firstOutput_ = graphOutput_;
	}

	/*	
	std::cout << "First output: " << std::endl;
	std::cout << firstOutput_ << std::endl;

	std::cout << "Last output: " << std::endl;
	std::cout << lastOutput_ << std::endl;
	*/

	std::cout << "Done" << std::endl;
	return 0;
}
