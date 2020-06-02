//
//TrainerUnitTests.cpp
//

#include "../../src/Trainer.h"
#include "../../src/Network.h"
#include "../../src/Neuron.h"
#include <iostream>
#include <vector>
#include <utility>
#include <forward_list>
#include <list>

int main(){
	std::list<int> results;
        std::cout << "\n\nTrainer Unit Tests\n\n";
        int test = 1;

	Network n = Network();
	Neuron d = Neuron();
	n.addNeuron(d);
	n.addNeuron(d);
	n.addInput(0);
	n.addOutput(1);
	std::forward_list<int> f = {1};
	std::pair<std::forward_list<int>,int> p;
	p.first = f;
	p.second = -1;
	std::vector<std::pair<std::forward_list<int>,int> > v;
	//v.push_back(p);
	p.second = 1;
	v.push_back(p);
	trainTargetLearning(n,v,1);

	Network tttFirstMove = Network();
	for(int i = 0;i < 19;i++){
		tttFirstMove.addNeuron(d);
	}
	for(int i = 0;i < 19;i++)
		tttFirstMove.addInput(18 - i);
	for(int i = 0;i < 9;i++){
		tttFirstMove.addNeuron(d);
	}
	for(int i = 0;i < 9;i++)
                tttFirstMove.addOutput(27 - i);
	std::forward_list<int> empty = {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	p.first = empty;
	p.second = 19;
	v.clear();
	v.push_back(p);
	std::forward_list<int> state0 = {0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0};
	p.first = state0;
	p.second = 23;
	v.push_back(p);
	std::forward_list<int> state1 = {0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0};
        p.first = state1;
        p.second = 23;
        v.push_back(p);
	std::forward_list<int> state2 = {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0};
        p.first = state2;
        p.second = 23;
        v.push_back(p);
	std::forward_list<int> state3 = {0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0};
        p.first = state3;
        p.second = 23;
        v.push_back(p);
	std::forward_list<int> state4 = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0};
        p.first = state4;
        p.second = 27;
        v.push_back(p);
	std::forward_list<int> state5 = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0};
        p.first = state5;
        p.second = 23;
        v.push_back(p);
	std::forward_list<int> state6 = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0};
        p.first = state6;
        p.second = 23;
        v.push_back(p);
	std::forward_list<int> state7 = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0};
        p.first = state7;
        p.second = 23;
        v.push_back(p);	
	std::forward_list<int> state8 = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1};
        p.first = state8;
        p.second = 23;
        v.push_back(p);
	trainTargetLearning(tttFirstMove,v,5);
}
