#include <iostream>
#include <vector>
#include <typeinfo>
#include "Matrix.h"
#include "Vector.h"
#include "Graph.h"
#include "Layers/ReluLayer.h"
#include "Layers/SigmoidLayer.h"

int main() {		
	Matrix<double> m1(4, 4);
	m1.RandomInitialization();

	Graph<double> g(m1);
	const auto& rl1 = g.AddLayer<Layers::ReluLayer<double>>();
	const auto& rl2 = g.AddLayer<Layers::SigmoidLayer<double>>();
	const auto& rl3 = g.AddLayer<Layers::SigmoidLayer<double>>();
	const auto& rl4 = g.AddLayer<Layers::ReluLayer<double>>();
	
	//g.RemoveLayer(rl3);
	const auto& graphOutput = g.Run();
	//const auto& graphOutput2 = g.Run2();

	/*
	std::cout << "Run:" << std::endl;
	std::cout << graphOutput << std::endl;
	std::cout << "==========" << std::endl;

	std::cout << "Run2:" << std::endl;
	std::cout << graphOutput2 << std::endl;
	std::cout << "==========" << std::endl;
	*/

	std::cout << "Done" << std::endl;
	return 0;
}
