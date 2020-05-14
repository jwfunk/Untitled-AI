#include "../../src/Network.h"
#include <iostream>
#include <list>
#include <iterator>
int main(){
	std::list<int> results;
	std::cout << "\n\nNetwork Unit Tests\n\n";

	Network n = Network();
	
	std::cout << "\t1 Default Constructor Test: ";
	if(n.info() == "Empty Network"){
		std::cout << "Passed\n";
		results.push_back(0);
	}
	else{
		std::cout << "Failed\n";
		results.push_back(1);
	}
	std::cout << "\n\t2 Add Neuron to empty Network: ";
	Neuron n1 = Neuron();
	n.addNeuron(n1);
	if(n.info() == "Charge: 0\nCritical Charge: 1\nPulse: 1\nRecievers: -1\n"){
		std::cout << "Passed\n";
		results.push_back(0);
	}
	else{
		std::cout << "Failed\n";
		results.push_back(1);
		std::cout << "Expected: Charge: 0\nCritical Charge: 1\nPulse: 1\nRecievers: -1\nActual: " << n.info() << "\n";
	}

	std::cout << "\t3 Resize of neurons array test: ";
	n = Network();
	for(int i = 0;i < INITSIZE;i++)
		n.addNeuron(n1);
	if(n.addNeuron(n1)){
		std::cout << "Failed\n";
		results.push_back(1);
	}
	else{
		std::cout << "Passed\n";
		results.push_back(0);
	}

	std::cout << "\t4 Adding max neurons to array test: ";
	n = Network();
	int fail = 0;
	for(int i = 0;i < MAXSIZE;i++){
		
		if(n.addNeuron(n1)){
			std::cout << "Failed\n";
			results.push_back(1);
			fail = 1;
			break;
		}
	}
	if(!fail){
		std::cout << "Passed\n";
		results.push_back(0);
	}

	std::cout << "\t5 Adding more than max neurons test: ";
	if(!n.addNeuron(n1)){
                std::cout << "Failed\n";
		results.push_back(1);
	}
        else{	
               	std::cout << "Passed\n";
		results.push_back(0);
	}

	std::cout << "\t6 Remove Neuron at index 0 and add it back: ";
	n.removeNeuron(0);
	if(n.addNeuron(n1)){
		std::cout << "Failed\n";
		results.push_back(1);
	}
	else{
		std::cout << "Passed\n";
		results.push_back(0);
	}

	std::cout << "\t7 Add Neuron to Maxed Network after it was refilled: ";

	if(n.addNeuron(n1)){
		std::cout << "Passed\n";
		results.push_back(0);
	}
	else{
		std::cout << "Failed\n";
		results.push_back(1);
	}

	std::cout << "\t8 Removing index 0 once and 1 twice: ";

	if(n.removeNeuron(0) || n.removeNeuron(1) || n.removeNeuron(1)){
		std::cout << "Failed\n";
		results.push_back(1);
	}
	else{
		std::cout << "Passed\n";
		results.push_back(0);
	}

	std::cout << "\t9 Adding back 3 Neurons: ";
	if(n.addNeuron(n1) || n.addNeuron(n1) || !n.addNeuron(n1)){
		std::cout << "Failed\n";
		results.push_back(1);
	}
	else{
		std::cout << "Passed\n";
		results.push_back(0);
	}

	int total = 0;
	int passed = 0;
	std::list <int> :: iterator it; 
	for(it = results.begin(); it != results.end(); ++it){
		if(!*it)
			passed++;
		total++;
	}
	std::cout << "\n\nSummary:\n\t" << passed << " out of " << total << " tests passed\n\n";
}
