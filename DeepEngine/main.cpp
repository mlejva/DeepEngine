#include <iostream>
#include <vector>
#include <typeinfo>
#include <memory>

#include "Matrix.h"
#include "Vector.h"
#include "Graph.h"
#include "Layers/ReluLayer.h"
#include "Layers/SigmoidLayer.h"

int main() {		
	// A row represents one input vector of length threeą
	Matrix<double> m1(1, 3); // 1x3

	m1.RandomInitialization();
	//m1.InitializeWithOnes();
	
	Graph<double> g(m1); // TODO: Pri inicializaci by se mel jeste predavat expected ouput, jako druhy parametr
	
	// Input layer is 1x3
	const auto& rl1 = g.AddLayer<Layers::SigmoidLayer<double>>(3); // 1x3
	const auto& rl2 = g.AddLayer<Layers::SigmoidLayer<double>>(4); // 1x4
	
	//g.RemoveLayer(rl3);
	const auto& graphOutput = g.Run();
	//const auto& graphOutput2 = g.Run2();

	
	std::cout << "Run:" << std::endl;
	std::cout << graphOutput << std::endl;
	std::cout << "==========" << std::endl;
	

	//std::cout << "Run:" << std::endl;
	//std::cout << graphOutput << std::endl;
	//std::cout << "==========" << std::endl;
	

	std::cout << "Done" << std::endl;
	return 0;
}
