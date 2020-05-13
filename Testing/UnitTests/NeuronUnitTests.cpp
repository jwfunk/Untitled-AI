#include "../../src/Neuron.h"
#include <iostream>
int main(){


	std::cout << "\n\nNeuron Unit Tests\n\n";

	//Constructor tests
	std::cout << "Default Constructor Test: ";
	Neuron n1 = Neuron();
	if(n1.info() == "Charge: 0\nCritical Charge: 1\nPulse: 1")
		std::cout << "Passed\n";
	else
		std::cout << "Failed\n";

	Neuron n2 = Neuron(10,5);


	std::cout << "2 Parameter Constructor Test: ";
	if(n2.info() == "Charge: 0\nCritical Charge: 10\nPulse: 5")
                std::cout << "Passed\n";
        else
                std::cout << "Failed\n";


	std::cout << "\nCopy Assignment Test: ";
	n1 = n2;
	n2 = Neuron(1,1);
	if(n1.info() == "Charge: 0\nCritical Charge: 10\nPulse: 5" && n2.info() == "Charge: 0\nCritical Charge: 1\nPulse: 1")
		std::cout << "Passed\n";
        else
                std::cout << "Failed\n";
}
