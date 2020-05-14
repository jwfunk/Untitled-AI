#include "../../src/Neuron.h"
#include <iostream>
int main(){


	std::cout << "\n\nNeuron Unit Tests\n\n";

	//Constructor tests
	std::cout << "\t1 Default Constructor Test: ";
	Neuron n1 = Neuron();
	if(n1.info() == "Charge: 0\nCritical Charge: 1\nPulse: 1\nRecievers: -1"){
		std::cout << "Passed\n";
	}
	else{
		std::cout << "Failed\n";
	}

	Neuron n2 = Neuron(10,5);


	std::cout << "\t2 Two Parameter Constructor Test: ";
	if(n2.info() == "Charge: 0\nCritical Charge: 10\nPulse: 5\nRecievers: -1"){
                std::cout << "Passed\n";
        }
	else{
                std::cout << "Failed\n";
	}

	std::cout << "\n\t3 Copy Assignment Test: ";
	n1 = n2;
	n2 = Neuron(1,1);
	if(n1.info() == "Charge: 0\nCritical Charge: 10\nPulse: 5\nRecievers: -1" && n2.info() == "Charge: 0\nCritical Charge: 1\nPulse: 1\nRecievers: -1"){
		std::cout << "Passed\n";
        }
	else{
                std::cout << "Failed\n";
	}

	std::cout << "\t4 Add reciever Test: ";
	n1.addReciever(2);
	if(n1.info() == "Charge: 0\nCritical Charge: 10\nPulse: 5\nRecievers: 2,-1"){
		std::cout << "Passed\n";
	}
	else{
		std::cout << "Failed\n";
	}

	std::cout << "\t5 Remove reciever Test: ";
	n1.removeReciever(2);
	if(n1.info() == "Charge: 0\nCritical Charge: 10\nPulse: 5\nRecievers: -1"){
                std::cout << "Passed\n";
        }
        else{
                std::cout << "Failed\n";
        }

	std::cout << "\t6 Remove nonexistant Reciever Test: ";
	n1.removeReciever(2);
        if(n1.info() == "Charge: 0\nCritical Charge: 10\nPulse: 5\nRecievers: -1"){
                std::cout << "Passed\n";
        }
        else{
                std::cout << "Failed\n";
        }
}
