#include "../../src/Network.h"
#include <iostream>

int main(){

	std::cout << "Network Unit Tests\n\n";

	Network n = Network();
	
	std::cout << "Default Constructor Test: ";
	if(n.info() == "Empty Network")
		std::cout << "Passed\n";
	else
		std::cout << "Failed\n";
}
