
#include <vector>
#include <typeinfo>
#include <memory>
#include <iostream>
#include <fstream>

#include "Matrix.h"
#include "Vector.h"
#include "Network.h"
#include "Layers/ReluLayer.h"
#include "Layers/SigmoidLayer.h"
#include "Layers/IdentityLayer.h"
#include "Layers/TanhLayer.h"
#include "Functions/LossFunctions/MSELossFunction.h"

typedef Functions::MSELossFunction<double> MSEDouble;
typedef std::vector<double> D_Vector;

int main() {

	double trueValue = 1.0;
	double falseValue = -1.0; 

	/*Matrix<double> input(4, 2);
	input.SetRow(0, D_Vector{trueValue, trueValue});
	input.SetRow(1, D_Vector{trueValue, falseValue});
	input.SetRow(2, D_Vector{falseValue, trueValue});
	input.SetRow(3, D_Vector{falseValue, falseValue});
	
	Matrix<double> expectedOutput(4, 1);
	expectedOutput.SetColumn(0, D_Vector{trueValue, falseValue, falseValue, falseValue});*/
	
	Network<double, MSEDouble> n;
	n.AddLayer<Layers::IdentityLayer<double>>(1); // Hidden layer	

	Matrix<double> input("/Users/vasekmlejnsky/Desktop/input.txt", ';');
	Matrix<double> expectedOutput("/Users/vasekmlejnsky/Desktop/expected.txt", ';');

	int i = 0;
	while (i < 10) {
		//std::cout << "Epoch: " << std::to_string(i) << std::endl;
		const auto& targets_ = n.Train(input, expectedOutput);
		auto& loss_ = std::get<0>(targets_);
		const auto& predictions_ = std::get<1>(targets_);
		std::cout << "\tLoss: " << std::to_string(loss_) << std::endl;
		/*std::cout << predictions_ << std::endl;*/
		/*std::cout << "===" << std::endl;*/
		++i;
	}

	/*std::ofstream lossFile("loss_out.txt");
	for (std::size_t epoch = 0; epoch < 30; ++epoch) {
		//std::cout << "Epoch: " << epoch << std::endl;
		for (std::size_t batch = 0; batch < 10; ++batch) {
			
			for (std::size_t i = 0; i < 500; ++i) {		
				const auto& xVal = Random(0, M_PI);		
				const auto& yVal = std::sin(xVal);

				//std::cout << "sin("<< xVal <<") = " << yVal << std::endl;
				input(i, 0) = xVal;
				expectedOutput(i, 0) = yVal;
			}

			const auto& targets_ = n.Train(input, expectedOutput);
			const auto& loss_ = std::get<0>(targets_);
			if (lossFile.is_open()) {				
				lossFile << loss_ << std::endl;
    		}   
			//const auto& networkOutput_ = std::get<1>(targets_);				

			//std::cout << "\tLoss: " << loss_ << "\t (batch " << batch << ")" << std::endl;
		}
	}
	lossFile.close();*/

	return 0;
}
