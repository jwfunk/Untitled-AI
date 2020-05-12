#include "../../src/Neuron.h"
#include <iostream>
int main(){

	//Constructor tests
	std::cout << "Default Constructor Test: ";
	Neuron n1 = Neuron();
	if(n1.info() == "Charge: 0\nCritical Charge: 1\nPulse: 1")
		std::cout << "Passed\n";
	else
		std::cout << "Failed\n";
}
