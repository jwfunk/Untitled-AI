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
        p.second = 19;
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
	std::forward_list<int> state9 = {0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0};
        p.first = state9;
        p.second = 27;
	v.push_back(p);
	std::forward_list<int> state10 = {0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0};
        p.first = state10;
        p.second = 27;
        v.push_back(p);
	std::forward_list<int> state11 = {0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0};
        p.first = state11;
        p.second = 27;
	v.push_back(p);
	std::forward_list<int> state12 = {0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0};
        p.first = state12;
        p.second = 27;
        v.push_back(p);
	std::forward_list<int> state13 = {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0};
        p.first = state13;
        p.second = 27;
	v.push_back(p);
	std::forward_list<int> state14 = {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0};
        p.first = state14;
        p.second = 27;
        v.push_back(p);
        std::forward_list<int> state15 = {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0};
        p.first = state15;
        p.second = 27;
        v.push_back(p);
	std::forward_list<int> state16 = {0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1};
        p.first = state16;
        p.second = 21;
        v.push_back(p);
	std::forward_list<int> state17 = {0,0,0,0,0,1,0,0,0,0,1,1,0,0,0,0,0,0,0};
        p.first = state17;
        p.second = 21;
        v.push_back(p);
	std::forward_list<int> state18 = {0,0,0,0,0,1,0,0,0,0,1,0,1,0,0,0,0,0,0};
        p.first = state18;
        p.second = 20;
        v.push_back(p);
	std::forward_list<int> state19 = {0,0,0,0,0,1,0,0,0,0,1,0,0,1,0,0,0,0,0};
        p.first = state19;
        p.second = 25;
        v.push_back(p);
	std::forward_list<int> state20 = {0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0};
        p.first = state20;
        p.second = 25;
	v.push_back(p);
	std::forward_list<int> state21 = {0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,1,0,0};
        p.first = state21;
        p.second = 22;
        v.push_back(p);
	std::forward_list<int> state22 = {0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,1,0};
        p.first = state22;
        p.second = 25;
        v.push_back(p);
	std::forward_list<int> state23 = {0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,1};
        p.first = state23;
        p.second = 20;
        v.push_back(p);
	std::forward_list<int> state24 = {0,1,0,0,0,0,0,0,0,1,0,1,1,0,0,0,0,0,0};
        p.first = state24;
        p.second = 23;
        v.push_back(p);
        std::forward_list<int> state25 = {0,1,0,0,0,0,0,0,0,1,0,1,0,1,0,0,0,0,0};
        p.first = state25;
        p.second = 23;
        v.push_back(p);
	std::forward_list<int> state26 = {0,1,0,0,0,0,0,0,0,1,0,1,0,0,1,0,0,0,0};
        p.first = state26;
        p.second = 26;
        v.push_back(p);
	std::forward_list<int> state27 = {0,1,0,0,0,0,0,0,0,1,0,1,0,0,0,1,0,0,0};
        p.first = state27;
        p.second = 23;
        v.push_back(p);
	std::forward_list<int> state28 = {0,1,0,0,0,0,0,0,0,1,0,1,0,0,0,0,1,0,0};
        p.first = state28;
        p.second = 23;
        v.push_back(p);
	std::forward_list<int> state29 = {0,1,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,1,0};
        p.first = state29;
        p.second = 23;
        v.push_back(p);
	std::forward_list<int> state30 = {0,1,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,1};
        p.first = state30;
        p.second = 23;
        v.push_back(p);
	std::forward_list<int> state31 = {0,1,0,0,0,0,0,0,0,1,0,0,1,1,0,0,0,0,0};
        p.first = state31;
        p.second = 23;
        v.push_back(p);
	std::forward_list<int> state32 = {0,1,0,0,0,0,0,0,0,1,0,0,1,0,1,0,0,0,0};
        p.first = state32;
        p.second = 25;
        v.push_back(p);
	std::forward_list<int> state33 = {0,1,0,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0};
        p.first = state33;
        p.second = 23;
        v.push_back(p);
	std::forward_list<int> state34 = {0,1,0,0,0,0,0,0,0,1,0,0,1,0,0,0,1,0,0};
        p.first = state34;
        p.second = 23;
        v.push_back(p);
	std::forward_list<int> state35 = {0,1,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,1,0};
        p.first = state35;
        p.second = 23;
        v.push_back(p);
	std::forward_list<int> state36 = {0,1,0,0,0,0,0,0,0,1,0,0,0,1,1,0,0,0,0};
        p.first = state36;
        p.second = 24;
        v.push_back(p);
	std::forward_list<int> state37 = {0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0};
        p.first = state37;
        p.second = 23;
        v.push_back(p);
	std::forward_list<int> state38 = {0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,1,0,0};
        p.first = state38;
        p.second = 23;
        v.push_back(p);
	std::forward_list<int> state39 = {0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0};
        p.first = state39;
        p.second = 23;
        v.push_back(p);
	std::forward_list<int> state40 = {0,1,0,0,0,0,0,0,0,1,0,0,0,0,1,1,0,0,0};
        p.first = state40;
        p.second = 22;
        v.push_back(p);
	std::forward_list<int> state41 = {0,1,0,0,0,0,0,0,0,1,0,0,0,0,1,0,1,0,0};
        p.first = state41;
        p.second = 21;
        v.push_back(p);
	std::forward_list<int> state42 = {0,1,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,1,0};
        p.first = state42;
        p.second = 20;
        v.push_back(p);
	std::forward_list<int> state43 = {0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,1,1,0,0};
        p.first = state43;
        p.second = 23;
        v.push_back(p);
	std::forward_list<int> state44 = {0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,1,0};
        p.first = state44;
        p.second = 23;
        v.push_back(p);
	std::forward_list<int> state45 = {0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,1,0};
        p.first = state45;
        p.second = 23;
        v.push_back(p);
	std::forward_list<int> state46 = {0,1,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1};
        p.first = state46;
        p.second = 20;
        v.push_back(p);
	std::forward_list<int> state47 = {0,1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1};
        p.first = state47;
        p.second = 20;
        v.push_back(p);
	std::forward_list<int> state48 = {0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1};
        p.first = state48;
        p.second = 20;
        v.push_back(p);
	std::forward_list<int> state49 = {0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1};
        p.first = state49;
        p.second = 20;
        v.push_back(p);
	std::forward_list<int> state50 = {0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1};
        p.first = state50;
        p.second = 20;
        v.push_back(p);
	std::forward_list<int> state51 = {0,1,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1};
        p.first = state51;
        p.second = 25;
        v.push_back(p);
	std::forward_list<int> state52 = {0,1,0,1,0,0,0,1,0,0,0,1,0,1,0,0,0,0,1};
        p.first = state52;
        p.second = 23;
        v.push_back(p);
	std::forward_list<int> state53 = {0,1,0,1,0,0,0,1,0,0,0,1,0,0,1,0,0,0,1};
        p.first = state53;
        p.second = 22;
        v.push_back(p);
	std::forward_list<int> state54 = {0,1,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,1};
        p.first = state54;
        p.second = 22;
        v.push_back(p);
	std::forward_list<int> state55 = {0,1,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,1,1};
        p.first = state55;
        p.second = 22;
        v.push_back(p);
	std::forward_list<int> state56 = {0,1,0,0,0,0,0,0,1,1,0,1,1,0,1,0,0,0,0};
        p.first = state56;
        p.second = 25;
        v.push_back(p);
	std::forward_list<int> state57 = {0,1,0,0,0,0,0,0,1,1,0,1,0,1,1,0,0,0,0};
        p.first = state57;
        p.second = 25;
        v.push_back(p);
	std::forward_list<int> state58 = {0,1,0,0,0,0,0,0,1,1,0,1,0,0,1,1,0,0,0};
        p.first = state58;
        p.second = 25;
        v.push_back(p);
	std::forward_list<int> state59 = {0,1,0,0,0,0,0,0,1,1,0,1,0,0,1,0,1,0,0};
        p.first = state59;
        p.second = 21;
        v.push_back(p);
	trainTargetLearning(tttFirstMove,v,1000);
}
