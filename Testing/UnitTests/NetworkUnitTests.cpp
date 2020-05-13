#include "../../src/Network.h"
#include <iostream>

int main(){

	std::cout << "\n\nNetwork Unit Tests\n\n";

	Network n = Network();
	
	std::cout << "Default Constructor Test: ";
	if(n.info() == "Empty Network")
		std::cout << "Passed\n";
	else
		std::cout << "Failed\n";

	std::cout << "\nAdd Neuron to empty Network: ";
	Neuron n1 = Neuron();
	n.addNeuron(n1);
	if(n.info() == "Charge: 0\nCritical Charge: 1\nPulse: 1\n")
		std::cout << "Passed\n";
	else
		std::cout << "Failed\n";

	std::cout << "Resize of neurons array test: ";
	n = Network();
	for(int i = 0;i < INITSIZE;i++)
		n.addNeuron(n1);
	if(n.addNeuron(n1))
		std::cout << "Failed\n";
	else
		std::cout << "Passed\n";

	std::cout << "Adding max neurons to array test: ";
	n = Network();
	int fail = 0;
	for(int i = 0;i < MAXSIZE;i++){
		
		if(n.addNeuron(n1)){
			std::cout << "Failed\n";
			fail = 1;
			break;
		}
	}
	if(!fail)
		std::cout << "Passed\n";

	std::cout << "Adding more than max neurons test: ";
	if(!n.addNeuron(n1)){
                std::cout << "Failed\n";
	}
        else{
	
               	std::cout << "Passed\n";
	}
}
