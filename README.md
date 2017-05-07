# Deep Engine

Deep Engine is a library for building simple feed-forward neural networks

Usage
==========
1. Create new network
```Cpp
#include "Network.h"
// When creating new network type and loss functions must be specified
#include "Functions/LossFunctions/MSELossFunction.h"

typedef Functions::MSELossFunction<double> MSEDouble;
Network<double, MSEDouble> myNetwork;
```

2. Add layers
```Cpp
// Include specific layer
#include "Layers/ReluLayer.h"
#include "Layers/SigmoidLayer.h"
#include "Layers/IdentityLayer.h"
#include "Layers/TanhLayer.h"

// Specify the number of neurons in a layer (i.e. size of output of a layer)
myNetwork.AddLayer<Layers::IdentityLayer<double>>(10); 
myNetwork.AddLayer<Layers::SigmoidLayer<double>>(2);
```

3. Train the network
```Cpp
#include "Matrix.h"

const char delimiter = ';';
Matrix<double> trainInput("/trainInput.txt", delimiter);
Matrix<double> trainExpectedOutput("/trainExpected.txt", delimiter);

int i = 0;
while (i < 100) {
    std::cout << "Epoch: " << std::to_string(i) << std::endl;

    const auto& targets = myNetwork.Train(trainInput, trainExpectedOutput);
    const auto& loss = std::get<0>(targets);
    const auto& predictions = std::get<1>(targets);

    std::cout << "Loss train: " << std::to_string(loss) << std::endl;
    std::cout << predictions << std::endl;
    
    i++;
}
```

4. Evaluate the network
```Cpp
Matrix<double> testInput("/testInput.txt", delimiter);
Matrix<double> testExpectedOutput("/testExpected.txt", delimiter);

const auto& testTargets = myNetwork.Evaluate(testInput, testExpectedOutput);
const auto& testLoss = std::get<0>(testTargets);
const auto& testPredictions = std::get<1>(testTargets);

std::cout << "Loss test: " << std::to_string(testLoss) << std::endl;
std::cout << testPredictions << std::endl;
```
