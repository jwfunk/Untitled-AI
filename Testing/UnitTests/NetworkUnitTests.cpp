#include "../../src/Network.h"
#include <iostream>
#include <list>
#include <iterator>
int main(){
	std::list<int> results;
	std::cout << "\n\nNetwork Unit Tests\n\n";
	int test = 1;
	Network n = Network();
	
	std::cout << "\t" << test << " Default Constructor Test: ";
	if(n.info() == "Empty Network"){
		std::cout << "Passed\n";
		results.push_back(0);
	}
	else{
		std::cout << "Failed\n";
		results.push_back(1);
	}

	test++;
	std::cout << "\n\t" << test << " Add Neuron to empty Network: ";
	Neuron n1 = Neuron();
	n.addNeuron(n1);
	if(n.info() == "Inputs: -1\nOutputs: -1\nNeuron: 0\nCharge: 0\nCritical Charge: 1\nPulse: 1\nRecievers: -1\n"){
		std::cout << "Passed\n";
		results.push_back(0);
	}
	else{
		std::cout << "Failed\n";
		results.push_back(1);
		std::cout << "Expected: \nInputs: -1\nOutputs: -1\nNeuron: 0\nCharge: 0\nCritical Charge: 1\nPulse: 1\nRecievers: -1\nActual: \n" << n.info() << "\n";
	}

	test++;
	std::cout << "\t" << test << " Resize of neurons array test: ";
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
	
	test++;
	std::cout << "\t" << test << " Adding max neurons to array test: ";
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

	test++;
	std::cout << "\t" << test << " Adding more than max neurons test: ";
	if(!n.addNeuron(n1)){
                std::cout << "Failed\n";
		results.push_back(1);
	}
        else{	
               	std::cout << "Passed\n";
		results.push_back(0);
	}

	test++;
	std::cout << "\t" << test << " Add AND structure to full neurons: ";
	if(n.addStructure(AND,0,0,0) == -1){
		std::cout << "Passed\n";
		results.push_back(0);
	}
	else{
		std::cout << "Failed\n";
                results.push_back(1);
	}

	test++;
	std::cout << "\t" << test << " Add OR structure to full neurons: ";
        if(n.addStructure(OR,0,0,0) == -1){
                std::cout << "Passed\n";
                results.push_back(0);
        }
        else{
                std::cout << "Failed\n";
                results.push_back(1);
        }

	test++;
	std::cout << "\t" << test << " Add NOT structure to full neurons: ";
	if(n.addStructure(NOT,0,0,0) == -1){
                std::cout << "Passed\n";
                results.push_back(0);
        }
        else{
                std::cout << "Failed\n";
                results.push_back(1);
        }

	test++;
        std::cout << "\t" << test << " Add XOR structure to full neurons: ";
        if(n.addStructure(XOR,0,0,0) == -1){
                std::cout << "Passed\n";
                results.push_back(0);
        }
        else{
                std::cout << "Failed\n";
                results.push_back(1);
        }

	test++;
	std::cout << "\t" << test << " Remove Neuron at index 0 and add it back: ";
	n.removeNeuron(0);
	if(n.addNeuron(n1)){
		std::cout << "Failed\n";
		results.push_back(1);
	}
	else{
		std::cout << "Passed\n";
		results.push_back(0);
	}
	
	test++;
	std::cout << "\t" << test << " Add Neuron to Maxed Network after it was refilled: ";

	if(n.addNeuron(n1)){
		std::cout << "Passed\n";
		results.push_back(0);
	}
	else{
		std::cout << "Failed\n";
		results.push_back(1);
	}

	test++;
	std::cout << "\t" << test << " Removing index 0 once and 1 twice: ";

	if(n.removeNeuron(0) || n.removeNeuron(1) || n.removeNeuron(1)){
		std::cout << "Failed\n";
		results.push_back(1);
	}
	else{
		std::cout << "Passed\n";
		results.push_back(0);
	}

	test++;
	std::cout << "\t" << test << " Adding back 3 Neurons: ";
	if(n.addNeuron(n1) || n.addNeuron(n1) || !n.addNeuron(n1)){
		std::cout << "Failed\n";
		results.push_back(1);
	}
	else{
		std::cout << "Passed\n";
		results.push_back(0);
	}

	test++;
	std::cout << "\t" << test << " Removing neuron that had inputs to it: ";
	Network nn = Network();
	Neuron n2 = Neuron();
	n2.addReciever(1);
	nn.addNeuron(n2);
	nn.addNeuron(n1);
	nn.removeNeuron(1);
	if(nn.info() == "Inputs: -1\nOutputs: -1\nNeuron: 0\nCharge: 0\nCritical Charge: 1\nPulse: 1\nRecievers: -1\n"){
		std::cout << "Passed\n";
                results.push_back(0);
	}
	else{
		std::cout << "Failed\n";
                results.push_back(1);
		std::cout << "Expected: \nInputs: -1\nOutoputs: -1\nNeuron: 0\nCharge: 0\nCritical Charge: 1\nPulse: 1\nRecievers: -1\nActual:\n" + nn.info() + "\n";
	}

	test++;
	std::cout << "\t" << test << " Adding good input: ";
	if(nn.addInput(0)){
		std::cout << "Failed\n";
                results.push_back(1);
	}
	else{
		std::cout << "Passed\n";
                results.push_back(0);
	}

	test++;
	std::cout << "\t" << test << " Adding bad input: ";
        if(!nn.addInput(1)){
                std::cout << "Failed\n";
                results.push_back(1);
        }
        else{
                std::cout << "Passed\n";
                results.push_back(0);
        }
	
	test++;
	std::cout << "\t" << test << " Removing input Neuron: ";
	nn.addNeuron(n1);
	nn.removeNeuron(0);
	if(nn.info() == "Inputs: -1\nOutputs: -1\nNeuron: 1\nCharge: 0\nCritical Charge: 1\nPulse: 1\nRecievers: -1\n"){
                std::cout << "Passed\n";
                results.push_back(0);
        }
        else{
                std::cout << "Failed\n";
                results.push_back(1);
                std::cout << "Expected:\nInputs: -1\nOutputs: -1\nNeuron: 1\nCharge: 0\nCritical Charge: 1\nPulse: 1\nRecievers: -1\nActual:\n" + nn.info() + "\n";
        }
	
	test++;
	std::cout << "\t" << test << " Adding good output: ";
	if(nn.addOutput(1)){
                std::cout << "Failed\n";
                results.push_back(1);
        }
        else{
                std::cout << "Passed\n";
                results.push_back(0);
        }

	test++;
	std::cout << "\t" << test << " Adding bad input: ";
        if(!nn.addOutput(0)){
                std::cout << "Failed\n";
                results.push_back(1);
        }
        else{
                std::cout << "Passed\n";
                results.push_back(0);
        }

	test++;
	std::cout << "\t" << test << " Removing output Neuron: ";
     
        nn.removeNeuron(1);
        if(nn.info() == "Empty Network"){
                std::cout << "Passed\n";
                results.push_back(0);
        }
        else{
                std::cout << "Failed\n";
                results.push_back(1);
                std::cout << "Expected:\nEmpty Network\nActual:\n" + nn.info() + "\n";
        }

	test++;
	std::cout << "\t" << test << " Processing 1 input and 1 output: ";
	nn = Network();
	n1 = Neuron();
	n1.addReciever(1);
	nn.addNeuron(n1);	
	n1.removeReciever(1);
	nn.addNeuron(n1);
	nn.addInput(0);
	nn.addOutput(1);	
	std::forward_list<int> *f = new std::forward_list<int>;
	f->push_front(1);
	if(nn.process(f) == 1){
                std::cout << "Passed\n";
                results.push_back(0);
        }
        else{
                std::cout << "Failed\n";
                results.push_back(1);
        }

	test++;
	std::cout << "\t" << test << " Processing 1 input and 0 output: ";
	nn = Network();
        n1 = Neuron();
        nn.addNeuron(n1);
        nn.addNeuron(n1);
        nn.addInput(0);
        nn.addOutput(1);
        
        if(nn.process(f) == -1){
                std::cout << "Passed\n";
                results.push_back(0);
        }
        else{
                std::cout << "Failed\n";
                results.push_back(1);
        }
	
	test++;
	std::cout << "\t" << test << " Checking if charges were cleared: ";

	if(nn.info() == "Inputs: 0,-1\nOutputs: 1,-1\nNeuron: 0\nCharge: 0\nCritical Charge: 1\nPulse: 1\nRecievers: -1\nNeuron: 1\nCharge: 0\nCritical Charge: 1\nPulse: 1\nRecievers: 1,-1\n"){
                std::cout << "Passed\n";
                results.push_back(0);
        }
        else{
                std::cout << "Failed\n";
                results.push_back(1);
                std::cout << "Expected:\nInputs: 0,-1\nOutputs: 1,-1\nNeuron: 0\nCharge: 0\nCritical Charge: 1\nPulse: 1\nRecievers: -1\nNeuron: 1\nCharge: 0\nCritical Charge: 1\nPulse: 1\nRecievers: 1,-1\nActual:\n" + nn.info() + "\n";
        }

	test++;
	std::cout << "\t" << test << " Processing with bad input list 1: ";
	nn = Network();
        n1 = Neuron();
        nn.addNeuron(n1);
        nn.addNeuron(n1);
        nn.addInput(0);
	nn.addInput(1);
        nn.addOutput(1);

	if(nn.process(f) == -1){
                std::cout << "Passed\n";
                results.push_back(0);
        }
        else{
                std::cout << "Failed\n";
                results.push_back(1);
        }

	test++;
	std::cout << "\t" << test << " Processing with bad input list 2: ";

	f->push_front(1);
	f->push_front(1);
	if(nn.process(f) == -1){
                std::cout << "Passed\n";
                results.push_back(0);
        }
        else{
                std::cout << "Failed\n";
                results.push_back(1);
        }

	test++;
	std::cout << "\t" << test << " Processing too long: ";

	nn = Network();
	n1 = Neuron(1,1000);
	n1.addReciever(1);
	nn.addNeuron(n1);
	n1 = Neuron(1,1);
	n1.addReciever(2);
	nn.addNeuron(n1);
	n1 = Neuron(MAXITERATIONS + 1,1);
	nn.addNeuron(n1);
	nn.addInput(0);
	nn.addOutput(2);	
	delete(f);
	f = new std::forward_list<int>;
	f->push_front(1);
	if(nn.process(f) == -1){
                std::cout << "Passed\n";
                results.push_back(0);
        }
        else{
                std::cout << "Failed\nExpected: -1\nActual: " << nn.process(f) << "\n";
                results.push_back(1);
        }
	
	test++;
	std::cout << "\t" << test << " Processing AND structure with 1 1 inputs: ";
	nn = Network();
	Neuron n3 = Neuron();
	nn.addNeuron(n3);
	nn.addNeuron(n3);
	nn.addNeuron(n3);	
	nn.addStructure(AND,0,1,2);
	nn.addInput(0);
	nn.addInput(1);
	nn.addOutput(2);
	f->push_front(1);
	if(nn.process(f) == 2){
                std::cout << "Passed\n";
                results.push_back(0);
        }
        else{
                std::cout << "Failed\n";
                results.push_back(1);
        }

	test++;
	std::cout << "\t" << test << " Processing AND structure with 1 0 inputs: ";
	f = new std::forward_list<int>;
	f->push_front(1);
	f->push_front(0);
	if(nn.process(f) == -1){
                std::cout << "Passed\n";
                results.push_back(0);
        }
        else{
                std::cout << "Failed\n";
                results.push_back(1);
        }

	test++;
	std::cout << "\t" << test << " Processing AND structure with 0 1 inputs: ";
        f = new std::forward_list<int>;
        f->push_front(0);
        f->push_front(1);
        if(nn.process(f) == -1){
                std::cout << "Passed\n";
                results.push_back(0);
        }
        else{
                std::cout << "Failed\n";
                results.push_back(1);
        }

	test++;
	std::cout << "\t" << test << " Processing AND structure with 0 0 inputs: ";
        f = new std::forward_list<int>;
        f->push_front(0);
        f->push_front(0);
        if(nn.process(f) == -1){
                std::cout << "Passed\n";
                results.push_back(0);
        }
        else{
                std::cout << "Failed\n";
                results.push_back(1);
        }

	test++;
	std::cout << "\t" << test << " Processing OR structure with 1 1 inputs: ";
        nn = Network();
        n3 = Neuron();
        nn.addNeuron(n3);
        nn.addNeuron(n3);
        nn.addNeuron(n3);
        nn.addStructure(OR,0,1,2);
        nn.addInput(0);
        nn.addInput(1);
        nn.addOutput(2);
	f = new std::forward_list<int>;
	f->push_front(1);
        f->push_front(1);	
        if(nn.process(f) == 2){
                std::cout << "Passed\n";
                results.push_back(0);
        }
        else{   
                std::cout << "Failed\n";
                results.push_back(1);
        }

	test++;
	std::cout << "\t" << test << " Processing OR structure with 1 0 inputs: ";
	f = new std::forward_list<int>;
        f->push_front(1);
        f->push_front(0);
        if(nn.process(f) == 2){
                std::cout << "Passed\n";
                results.push_back(0);
        }
        else{
                std::cout << "Failed\n";
                results.push_back(1);
        }

	test++;
	std::cout << "\t" << test << " Processing OR structure with 0 1 inputs: ";
        f = new std::forward_list<int>;
        f->push_front(0);
        f->push_front(1);
        if(nn.process(f) == 2){
                std::cout << "Passed\n";
                results.push_back(0);
        }
        else{
                std::cout << "Failed\n";
                results.push_back(1);
        }

	test++;
	std::cout << "\t" << test << " Processing OR structure with 0 0 inputs: ";
        f = new std::forward_list<int>;
        f->push_front(0);
        f->push_front(0);
        if(nn.process(f) == -1){
                std::cout << "Passed\n";
                results.push_back(0);
        }
        else{
                std::cout << "Failed\n";
                results.push_back(1);
        }

	test++;
	std::cout << "\t" << test << " Processing NOT structure with 1 inputs: ";
        nn = Network();
        n3 = Neuron();
        nn.addNeuron(n3);
	n3.addReciever(2);
        nn.addNeuron(n3);
	n3.removeReciever(2);
	nn.addNeuron(n3);
        nn.addStructure(NOT,0,1,2);
        nn.addInput(0);
	nn.addInput(1);
        nn.addOutput(2);
        f = new std::forward_list<int>;
        f->push_front(1);
        f->push_front(1);
        if(nn.process(f) == -1){
                std::cout << "Passed\n";
                results.push_back(0);
        }
        else{
                std::cout << "Failed\n";
                results.push_back(1);
        }

	test++;
	std::cout << "\t" << test << " Processing NOT structure with 0 inputs: ";
        f = new std::forward_list<int>;
        f->push_front(0);
        f->push_front(1);
        if(nn.process(f) == 2){
                std::cout << "Passed\n";
                results.push_back(0);
        }
        else{
                std::cout << "Failed\n";
                results.push_back(1);
        }

	test++;
	std::cout << "\t" << test << " Processing XOR structure with 1 1 inputs: ";
	nn = Network();
	n3 = Neuron();
	nn.addNeuron(n3);
	nn.addNeuron(n3);
	nn.addNeuron(n3);
	nn.addStructure(XOR,0,1,2);
	nn.addInput(0);
	nn.addInput(1);
	nn.addOutput(2);
	f = new std::forward_list<int>;
        f->push_front(1);
        f->push_front(1);
	if(nn.process(f) == -1){
                std::cout << "Passed\n";
                results.push_back(0);
        }
        else{
                std::cout << "Failed\n";
                results.push_back(1);
        }

	test++;
        std::cout << "\t" << test << " Processing XOR structure with 1 0 inputs: ";
        f = new std::forward_list<int>;
        f->push_front(1);
        f->push_front(0);
        if(nn.process(f) == 2){
                std::cout << "Passed\n";
                results.push_back(0);
        }
        else{
                std::cout << "Failed\n";
                results.push_back(1);
        }

        test++;
        std::cout << "\t" << test << " Processing XOR structure with 0 1 inputs: ";
        f = new std::forward_list<int>;
        f->push_front(0);
        f->push_front(1);
        if(nn.process(f) == 2){
                std::cout << "Passed\n";
                results.push_back(0);
        }
        else{
                std::cout << "Failed\n";
                results.push_back(1);
        }

        test++;
        std::cout << "\t" << test << " Processing XOR structure with 0 0 inputs: ";
        f = new std::forward_list<int>;
        f->push_front(0);
        f->push_front(0);
        if(nn.process(f) == -1){
                std::cout << "Passed\n";
                results.push_back(0);
        }
        else{
                std::cout << "Failed\n";
                results.push_back(1);
        }

	test++;
        std::cout << "\t" << test << " Processing PAND structure with 1 input: ";
        nn = Network();
        n3 = Neuron();
        nn.addNeuron(n3);
	n3.addReciever(2);
        nn.addNeuron(n3);
        nn.addNeuron(n3);
        nn.addStructure(PAND,0,1,2);	
        nn.addInput(0);
        nn.addInput(1);
        nn.addOutput(2);
	f = new std::forward_list<int>;
        f->push_front(1);
	f->push_front(1);
        if(nn.process(f) == 2){
                std::cout << "Passed\n";
                results.push_back(0);
        }
        else{
                std::cout << "Failed\n";
                results.push_back(1);
        }

	test++;
        std::cout << "\t" << test << " Processing PAND structure with 0 input: ";
	f = new std::forward_list<int>;
        f->push_front(0);
        f->push_front(1);
	if(nn.process(f) == -1){
                std::cout << "Passed\n";
                results.push_back(0);
        }
        else{
                std::cout << "Failed\n";
                results.push_back(1);
        }

	test++;
        std::cout << "\t" << test << " Processing POR structure with 1 input: ";
        nn = Network();
        n3 = Neuron();
        nn.addNeuron(n3);
        n3.addReciever(2);
        nn.addNeuron(n3);
        nn.addNeuron(n3);
        nn.addStructure(POR,0,1,2);
        nn.addInput(0);
        nn.addInput(1);
        nn.addOutput(2);
        f = new std::forward_list<int>;
        f->push_front(1);
        f->push_front(1);
        if(nn.process(f) == 2){
                std::cout << "Passed\n";
                results.push_back(0);
        }
        else{
                std::cout << "Failed\n";
                results.push_back(1);
        }

        test++;
        std::cout << "\t" << test << " Processing POR structure with 0 input: ";
        f = new std::forward_list<int>;
        f->push_front(0);
        f->push_front(1);
        if(nn.process(f) == 2){
                std::cout << "Passed\n";
                results.push_back(0);
        }
        else{
                std::cout << "Failed\n";
                results.push_back(1);
        }

	test++;
        std::cout << "\t" << test << " Testing assignment operator: ";
	Network t1 = Network();
	Network t2 = Network();
	t2 = t1;
	t1.addNeuron(n3);
	t2.addNeuron(n3);
	t2.mutate();
	std::cout << t1;

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
