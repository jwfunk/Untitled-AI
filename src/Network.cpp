
//Network.cpp

#include "Network.h"
#include <iterator>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <fstream>


Network::Network(){
	size = 0;
}

std::ostream& operator<<(std::ostream &os, const Network &i){

	os << i.info();
	return os;
}

const std::string Network::info() const {
	if(size == 0)
		return "Empty Network";
	std::string r = "Inputs: ";
	
	for(auto it = inputs.begin(); it != inputs.end(); ++it)
		r += std::to_string(*it) + " ";
	r += "-1\nOutputs: ";
	for(auto it = outputs.begin(); it != outputs.end(); ++it)
		r += std::to_string(*it) + " ";
	r += "-1\n";
	int e = 0;	
	for(int i = 0;i < size;i++){
			if((index[i / 32] & (1<<(i % 32)))){
				r += "Neuron: " + std::to_string(i) + "\n" + neurons[i].info() + "\n";
				e = 1;	
			}
	}
	if(e == 0)
		return "Empty Network";	
	return r;
}

void Network::save(std::string file){
	std::fstream fs;
	fs.open(file,std::ios::out);	
	fs << (*this).info();
}

int Network::load(std::string file){
	(*this) = Network();
	std::fstream fs;
	fs.open(file,std::ios::in);
	std::string buffer;
	fs >> buffer;
	int i;
	fs >> i;
	std::forward_list<int> in;
	while(i != -1){
		in.push_front(i);
		fs >> i;
	}
	fs >> buffer;	
	std::forward_list<int> ou;
	fs >> i;	
	while(i != -1){
		ou.push_front(i);
		fs >> i;
	}
	neurons = new Neuron[INITSIZE];
        index = new int[INITSIZE / 32];
        for(int i = 0;i < INITSIZE / 32;i++)
        	index[i] = 0;
        size = INITSIZE;
        available = std::stack<int>();
	while(!fs.eof()){
		fs >> buffer;
		int loc;
		fs >> loc;
		fs >> buffer;
		int charge;
		fs >> charge;
		fs >> buffer;
		fs >> buffer;
		int ccharge;
		fs >> ccharge;
		fs >> buffer;
		int pulse;
		fs >> pulse;
		std::forward_list<int> rec;
		fs >> buffer;
		fs >> i;
		while(i != -1){
			rec.push_front(i);
			fs >> i;
		}
		if(loc >= size){
			int oldSize = size;
			while(loc >= size)
				size *= 2;
			int *newindex = new int[size / 32];
                        Neuron *newneurons = new Neuron[size];
                        for(int i = 0;i < oldSize;i++)
                                newneurons[i] = neurons[i];
                        for(int i = 0;i < oldSize;i++){
                                newindex[i] = index[i]; 
                        }
			for(int i = oldSize;i < size;i++){
				newindex[i] = 0;
			}
                        delete[] neurons;
                        delete[] index;
                        neurons = newneurons;
                        index = newindex;	
		}
		neurons[loc].charge = charge;
		neurons[loc].criticalCharge = ccharge;
		neurons[loc].pulse = pulse;
		for(auto it = rec.begin();it != rec.end();++it)
			neurons[loc].addReciever(*it);
		index[loc / 32] |= 1<<(loc % 32);
	}
	for(int i = size - 1;i >= 0;i--)
		if(!(index[i / 32] & 1<<(i % 32)))
			available.push(i);
	for(auto it = in.begin();it != in.end();++it)
		(*this).addInput(*it);
	for(auto it = ou.begin();it != ou.end();++it)
                (*this).addOutput(*it);
	return 0;			
}

int Network::addStructure(structure s, int i1, int i2, int i3){
	switch(s){
		case AND://pulses if i1 and i2 pulse
		{
			Neuron input = Neuron(neurons[i1].pulse + neurons[i2].pulse,neurons[i3].criticalCharge);
			int loc = (*this).nextLocation();
			if(loc == -1)
				return -1;
			neurons[i1].addReciever(loc);
			neurons[i2].addReciever(loc);
			input.addReciever(i3);
			(*this).addNeuron(input);
			return 0;
		}
		case OR://pulses if i1 or i2 pulses
		{
			int min;
			if(neurons[i1].pulse > neurons[i2].pulse)
				min = neurons[i2].pulse;
			else
				min = neurons[i1].pulse;
			Neuron input = Neuron(min,neurons[i3].criticalCharge);
                        int loc = (*this).nextLocation();
			if(loc == -1)
				return -1;
                        neurons[i1].addReciever(loc);
                        neurons[i2].addReciever(loc);
                        input.addReciever(i3);
                        (*this).addNeuron(input);
		        return 0;
		}
		case NOT://if i1 pulses negates i3 from pulsing
		{
			Neuron input = Neuron(neurons[i1].pulse,0 - neurons[i3].criticalCharge);
			int loc = (*this).nextLocation();
                        if(loc == -1)
                                return -1;
			neurons[i1].addReciever(loc);
			input.addReciever(i3);
			(*this).addNeuron(input);
			return 0;
		}
		case XOR://if i1 or i2 pulses alone i3 is pulsed
		{//CLean this up
			std::vector<int> input = nextLocations(7);
			if(input.empty())
				return -1;
			int loc1 = input[0];
			int loc2 = input[1];
			int loc3 = input[2];
			int loc4 = input[3];
			int loc5 = input[4];
			int loc6 = input[5];
			int loc7 = input[6];

			Neuron input1 = Neuron(neurons[i1].pulse,3);
			Neuron input2 = Neuron(neurons[i2].pulse,3);
			
			neurons[i1].addReciever(loc1);
			(*this).addNeuron(input1);
			
			neurons[i2].addReciever(loc2);
			(*this).addNeuron(input2);
			Neuron n3 = Neuron(1,1);
			
			neurons[loc1].addReciever(loc3);
			(*this).addNeuron(n3);
			
			Neuron n4 = Neuron(1,1);
			neurons[loc2].addReciever(loc4);
			(*this).addNeuron(n4);
			Neuron n5 = Neuron(1,-3);
			
			n5.addReciever(loc4);
			neurons[loc1].addReciever(loc5);
			(*this).addNeuron(n5);
			
			Neuron n6 = Neuron(1,-3);
			n6.addReciever(loc3);
			neurons[loc2].addReciever(loc6);
			(*this).addNeuron(n6);
			
			Neuron n7 = Neuron(3,neurons[i3].criticalCharge);
			n7.addReciever(i3);
			(*this).addNeuron(n7);
			neurons[loc3].addReciever(loc7);
			neurons[loc4].addReciever(loc7);
			return 0;
		}
		case PAND:
		{
			neurons[i1].addReciever(i3);
			neurons[i3].criticalCharge += neurons[i1].pulse;
			return 0;
		}	
		case POR:
		{
			neurons[i1].addReciever(i3);
			if(neurons[i3].criticalCharge > neurons[i1].pulse)
				neurons[i3].criticalCharge = neurons[i1].pulse;
			return 0;
		}
		default:
		{
			break;
		}
	}
	return -1;
}

int Network::addNeuron(const Neuron n){
	if(size == 0){
		neurons = new Neuron[INITSIZE];
		neurons[0] = n;
		index = new int[INITSIZE / 32];
		for(int i = 0;i < INITSIZE / 32;i++)
			index[i] = 0;
		index[0] = 1;
		size = INITSIZE;
		available = std::stack<int>();
		for(int i = INITSIZE - 1;i > 0;i--){
			available.push(i);
		}
		return 0;
	}
	if(!available.empty()){
		neurons[available.top()] = n;
		index[available.top() / 32] |= 1<<(available.top() % 32);
		available.pop();
		return 0;
	}
	else{
		if(size < MAXSIZE){
			size *= 2;
			int *newindex = new int[size / 32];
			Neuron *newneurons = new Neuron[size];
			for(int i = 0;i < size / 2;i++)
                        	newneurons[i] = neurons[i];
			for(int i = 0;i < size / 64;i++){
                	        newindex[i] = index[i];
                	        newindex[i + size / 64] = 0;
                	}
			delete[] neurons;
			delete[] index;
			neurons = newneurons;
			index = newindex;
			for(int i = 0;i < size / 2;i++){
				available.push(size - i - 1);
			}
                	return this->addNeuron(n);
		}
	}
	return 1;
}

int Network::removeNeuron(int i){
	if(i >= size || i < 0)
		return 1;
	if(index[i / 32] & 1<<(i % 32)){
		index[i / 32] &= ~(1<<(i % 32));
		inputs.remove(i);
		outputs.remove(i);
		for(int j = 0;j < size;j++){
			if(index[j / 32] & 1<<(j % 32))
				neurons[j].removeReciever(i);
		}
		available.push(i);
	}
	return 0;
	
}

int Network::addInput(int i){
	if(i >= size || i < 0)
                return 1;
	if(index[i / 32] & 1<<(i % 32)){
		inputs.push_front(i);
		return 0;
	}
	return 1;
}

int Network::removeInput(int i){
	inputs.remove(i);
	return 0;
}

int Network::addOutput(int i){
	if(i >= size || i < 0)
                return 1;
        if(index[i / 32] & 1<<(i % 32)){
                outputs.push_front(i);
		neurons[i].pulse = neurons[i].criticalCharge;
		neurons[i].addReciever(i);
                return 0;
        }
        return 1;
}

int Network::removeOutput(int i){
	outputs.remove(i);
	return 0;
}

int Network::process(std::forward_list<int> *given){

	//Activate input Neurons
	
	std::forward_list <int> :: iterator it2;
	std::queue <int> active;
	it2 = given->begin();
	for(auto it1 = inputs.begin(); it1 != inputs.end(); ++it1){
		if(it2 == given->end()){
			(*this).clear();//clear the network
			return -1;
		}
		if(*it2){
			neurons[*it1].charge = neurons[*it1].criticalCharge;
			active.push(*it1);
		}
		++it2;
	}
	if(it2 != given->end()){
		(*this).clear();//clear the network

		return -1;
	}
	active.push(-1);
	int iterations = 0;
	
	//process the data
	
	while(!active.empty()){
		int i = active.front();
		active.pop();
		if(i == -1){
			for(auto it1 = outputs.begin();it1 != outputs.end();++it1)//possible improvement here
                         	if(neurons[*it1].charge >= neurons[*it1].criticalCharge){
                                 	(*this).clear();//clear network charges
                                 	return *it1;
                         	}
		}
		else{
		if(neurons[i].charge >= neurons[i].criticalCharge){
				//std::cout << *i << "\n";
				for(auto it1 = neurons[i].recievers.begin();it1 != neurons[i].recievers.end();++it1){
                	                neurons[*it1].charge += neurons[i].pulse;
					active.push(*it1);
                	        }
                	        neurons[i].charge -= neurons[i].criticalCharge;
				if(neurons[i].charge >= neurons[i].criticalCharge)
					active.push(i);
				active.push(-1);
			}
		}
		
		if(iterations > MAXITERATIONS){
			(*this).clear();
        		return -1;
		}
		iterations++;
		
	}

	(*this).clear();	
	return -1;
}

void Network::mutateTarget(std::pair<std::forward_list<int>, int >& target){
	std::vector<int> iTree;
	std::vector<int> oTree;
	iTree.clear();
	oTree.clear();
	inputTree(iTree,target);
	int present = 0;
	int inputSize = 0;
	int outputSize = 0;
	for(auto it = iTree.begin();it != iTree.end();++it){
		if(*it == target.second)
			present = 1;
		++inputSize;
	}
	int choice = 0;
	if(present){
		int mutation = std::rand() % (4 * inputSize + 7);
                if(mutation < inputSize)
                        choice = 2;
                if(mutation >= inputSize && mutation < (2 * inputSize))
                        choice = 3;
                if(mutation >= (2 * inputSize) && mutation < (3 * inputSize))
                        choice = 10;
                if(mutation >= (3 * inputSize) && mutation < (4 * inputSize))
                        choice = 1;
                if(mutation == (4 * inputSize))
                        choice = 0;
                if(mutation == (4 * inputSize + 1))
                        choice = 4;
                if(mutation == (4 * inputSize + 2))
                        choice = 5;
                if(mutation == (4 * inputSize + 3))
                        choice = 6;
                if(mutation == (4 * inputSize + 4))
                        choice = 7;
                if(mutation == (4 * inputSize + 5))
                        choice = 8;
                if(mutation == (4 * inputSize + 6))
                        choice = 9;				
	}
	else{
		outputTree(oTree,target.second);
		for(auto it = oTree.begin();it != oTree.end();++it){
        	        ++outputSize;
        	}
		int mutation = std::rand() % (size + 5);
                if(mutation < size)
                        choice = 10;
                if(mutation == size)
                        choice = 0;
                if(mutation == (size + 1))
                        choice = 4;
                if(mutation == (size + 2))
                        choice = 5;
                if(mutation == (size + 3))
                        choice = 6;
                if(mutation == (size + 4))
                        choice = 8;
                if(mutation == (size + 5))
                        choice = 9;
	}
		switch(choice){
			case 0://add Neuron
			{
				Neuron n = Neuron();
				int reciever;
				if(present){
                                        reciever = iTree[std::rand() % inputSize];
                                        while(!(index[reciever / 32] & 1<<(reciever % 32)))
                                                reciever = iTree[std::rand() % inputSize];
                                }
                                else{
                                        reciever = oTree[std::rand() % outputSize];
                                        while(!(index[reciever / 32] & 1<<(reciever % 32)))
                                                reciever = oTree[std::rand() % outputSize];
                                }
				n.addReciever(reciever);
				int sender = iTree[std::rand() % inputSize];
                                while(!(index[sender / 32] & 1<<(sender % 32)))
                                        sender = iTree[std::rand() % inputSize];
				neurons[sender].addReciever((*this).nextLocation());
				(*this).addNeuron(n);
				return;
			}
			case 1://remove Neuron
			{
				/*
				int remove = iTree[std::rand() % inputSize];
				if(!anyExpendable())
					return;		
				while(!(index[remove / 32] & 1<<(remove % 32)) || !expendable(remove)){
                                        remove = iTree[std::rand() % inputSize];
				}
				(*this).removeNeuron(remove);
				*/
				return;
			}
			case 2://modify cc
			{
				int modify = iTree[std::rand() % inputSize];
				while(!(index[modify / 32] & 1<<(modify % 32)))
					modify = iTree[std::rand() % inputSize];
				if((std::rand() % 2))
					neurons[modify].criticalCharge += 1;
				else{
					if(neurons[modify].criticalCharge != 1)
						neurons[modify].criticalCharge -= 1;
				}
				return;
			}
			case 3://modify pulse
			{
				int modify = iTree[std::rand() % inputSize];
                                while(!(index[modify / 32] & 1<<(modify % 32)))
                                        modify = iTree[std::rand() % inputSize];
                                if((std::rand() % 2))
                                        neurons[modify].pulse += 1;
                                else
                                        neurons[modify].pulse -= 1;
				return;
			}
			case 4://add AND structure
			{
				int sender1 = iTree[std::rand() % inputSize];
				while(!(index[sender1 / 32] & 1<<(sender1 % 32)))
                                        sender1 = iTree[std::rand() % inputSize];
				int sender2 = iTree[std::rand() % inputSize];
				while(!(index[sender2 / 32] & 1<<(sender2 % 32)))
                                        sender2 = iTree[std::rand() % inputSize];
				int reciever;
				if(present){
					reciever = iTree[std::rand() % inputSize];
					while(!(index[reciever / 32] & 1<<(reciever % 32)))
                                	        reciever = iTree[std::rand() % inputSize];
				}
				else{
					reciever = oTree[std::rand() % outputSize];
                                        while(!(index[reciever / 32] & 1<<(reciever % 32)))
                                                reciever = oTree[std::rand() % outputSize];
				}
				(*this).addStructure(AND,sender1,sender2,reciever);
				return;
			}
			case 5://add OR structure
			{
				int sender1 = iTree[std::rand() % inputSize];
                                while(!(index[sender1 / 32] & 1<<(sender1 % 32)))
                                        sender1 = iTree[std::rand() % inputSize];
                                int sender2 = iTree[std::rand() % inputSize];
                                while(!(index[sender2 / 32] & 1<<(sender2 % 32)))
                                        sender2 = iTree[std::rand() % inputSize];
				int reciever;
                                if(present){
                                        reciever = iTree[std::rand() % inputSize];
                                        while(!(index[reciever / 32] & 1<<(reciever % 32)))
                                                reciever = iTree[std::rand() % inputSize];
                                }
                                else{
                                        reciever = oTree[std::rand() % outputSize];
                                        while(!(index[reciever / 32] & 1<<(reciever % 32)))
                                                reciever = oTree[std::rand() % outputSize];
                                }
                                (*this).addStructure(OR,sender1,sender2,reciever);
				return;
			}
			case 6://add XOR structure
			{
				int sender1 = iTree[std::rand() % inputSize];
                                while(!(index[sender1 / 32] & 1<<(sender1 % 32)))
                                        sender1 = iTree[std::rand() % inputSize];
                                int sender2 = iTree[std::rand() % inputSize];
                                while(!(index[sender2 / 32] & 1<<(sender2 % 32)))
                                        sender2 = iTree[std::rand() % inputSize];
				int reciever;
                                if(present){
                                        reciever = iTree[std::rand() % inputSize];
                                        while(!(index[reciever / 32] & 1<<(reciever % 32)))
                                                reciever = iTree[std::rand() % inputSize];
                                }
                                else{
                                        reciever = oTree[std::rand() % outputSize];
                                        while(!(index[reciever / 32] & 1<<(reciever % 32)))
                                                reciever = oTree[std::rand() % outputSize];
                                }
                                (*this).addStructure(XOR,sender1,sender2,reciever);
				return;
			}
			case 7://add NOT structure
			{
				int sender1 = iTree[std::rand() % inputSize];
                                while(!(index[sender1 / 32] & 1<<(sender1 % 32)))
                                        sender1 = iTree[std::rand() % inputSize];
                                int reciever = iTree[std::rand() % inputSize];
                                while(!(index[reciever / 32] & 1<<(reciever % 32)))
                                        reciever = iTree[std::rand() % inputSize];
                                (*this).addStructure(NOT,sender1,0,reciever);
				return;
			}
			case 8://add POR structure
			{
                                int sender1 = iTree[std::rand() % inputSize];
                                while(!(index[sender1 / 32] & 1<<(sender1 % 32)))
                                        sender1 = iTree[std::rand() % inputSize];
				int reciever;
                                if(present){
                                        reciever = iTree[std::rand() % inputSize];
                                        while(!(index[reciever / 32] & 1<<(reciever % 32)))
                                                reciever = iTree[std::rand() % inputSize];
                                }
                                else{
                                        reciever = oTree[std::rand() % outputSize];
                                        while(!(index[reciever / 32] & 1<<(reciever % 32)))
                                                reciever = oTree[std::rand() % outputSize];
                                }
                                (*this).addStructure(POR,sender1,0,reciever);
				return;
                        }
			case 9://add PAND structure
			{
                                int sender1 = iTree[std::rand() % inputSize];
                                while(!(index[sender1 / 32] & 1<<(sender1 % 32)))
                                        sender1 = iTree[std::rand() % inputSize];
				int reciever;
                                if(present){
                                        reciever = iTree[std::rand() % inputSize];
                                        while(!(index[reciever / 32] & 1<<(reciever % 32)))
                                                reciever = iTree[std::rand() % inputSize];
                                }
                                else{
                                        reciever = oTree[std::rand() % outputSize];
                                        while(!(index[reciever / 32] & 1<<(reciever % 32)))
                                                reciever = oTree[std::rand() % outputSize];
                                }
                                (*this).addStructure(PAND,sender1,0,reciever);
				return;
                        }
			case 10://add reciever
			{
				int sender = std::rand() % size;
                                while(!(index[sender / 32] & 1<<(sender % 32)))
                                        sender = std::rand() % size;
				int reciever;
                                if(present){
                                        reciever = iTree[std::rand() % inputSize];
                                        while(!(index[reciever / 32] & 1<<(reciever % 32)))
                                                reciever = iTree[std::rand() % inputSize];
                                }
                                else{
                                        reciever = oTree[std::rand() % outputSize];
                                        while(!(index[reciever / 32] & 1<<(reciever % 32))){
                                                reciever = oTree[std::rand() % outputSize];
					}
                                }
                                neurons[sender].addReciever(reciever);
                                return;
			}
		}		
            
}

void Network::mutate(int choice = -1){
	if(choice == -1){
		int mutation = std::rand() % (5 * size + 7);
                if(mutation < size)
                        choice = 2;
                if(mutation >= size && mutation < (2 * size))
                        choice = 3; 
                if(mutation >= (2 * size) && mutation < (3 * size))
                        choice = 10;
                if(mutation >= (3 * size) && mutation < (4 * size))
                        choice = 1;
		if(mutation >= (4 * size) && mutation < (5 * size))
                        choice = 11; 
                if(mutation == (5 * size))
                        choice = 0; 
                if(mutation == (5 * size + 1))
                        choice = 4; 
                if(mutation == (5 * size + 2))
                        choice = 5; 
                if(mutation == (5 * size + 3))
                        choice = 6; 
                if(mutation == (5 * size + 4))
                        choice = 7; 
                if(mutation == (5 * size + 5))
                        choice = 8; 
                if(mutation == (5 * size + 6))
                        choice = 9;
		if(mutation == (5 * size + 7))
			choice = 12;
	}
	
		
		switch(choice){
			case 0://add Neuron
			{
				Neuron n = Neuron();
				int reciever = std::rand() % size;
				while(!(index[reciever / 32] & 1<<(reciever % 32)))
					reciever = std::rand() % size;
				n.addReciever(reciever);
				int sender = std::rand() % size;
                                while(!(index[sender / 32] & 1<<(sender % 32)))
                                        sender = std::rand() % size;
				neurons[sender].addReciever((*this).nextLocation());
				(*this).addNeuron(n);
				return;
			}
			case 1://remove Neuron
			{
				if(!anyExpendable())
					return;		
				int remove = std::rand() % size;
				while(!(index[remove / 32] & 1<<(remove % 32)) || !expendable(remove)){
                                        remove = std::rand() % size;
				}
				(*this).removeNeuron(remove);
				return;
			}
			case 2://modify cc
			{
				int modify = std::rand() % size;
				while(!(index[modify / 32] & 1<<(modify % 32)))
					modify = std::rand() % size;
				if((std::rand() % 2))
					neurons[modify].criticalCharge += 1;
				else{
					if(neurons[modify].criticalCharge != 1)
						neurons[modify].criticalCharge -= 1;
				}
				return;
			}
			case 3://modify pulse
			{
				int modify = std::rand() % size;
                                while(!(index[modify / 32] & 1<<(modify % 32)))
                                        modify = std::rand() % size;
                                if((std::rand() % 2))
                                        neurons[modify].pulse += 1;
                                else
                                        neurons[modify].pulse -= 1;
				return;
			}
			case 4://add AND structure
			{
				int sender1 = std::rand() % size;
				while(!(index[sender1 / 32] & 1<<(sender1 % 32)))
                                        sender1 = std::rand() % size;
				int sender2 = std::rand() % size;
				while(!(index[sender2 / 32] & 1<<(sender2 % 32)))
                                        sender2 = std::rand() % size;
				int reciever = std::rand() % size;
				while(!(index[reciever / 32] & 1<<(reciever % 32)))
                                        reciever = std::rand() % size;
				(*this).addStructure(AND,sender1,sender2,reciever);
				return;
			}
			case 5://add OR structure
			{
				int sender1 = std::rand() % size;
                                while(!(index[sender1 / 32] & 1<<(sender1 % 32)))
                                        sender1 = std::rand() % size;
                                int sender2 = std::rand() % size;
                                while(!(index[sender2 / 32] & 1<<(sender2 % 32)))
                                        sender2 = std::rand() % size;
                                int reciever = std::rand() % size;
                                while(!(index[reciever / 32] & 1<<(reciever % 32)))
                                        reciever = std::rand() % size;
                                (*this).addStructure(OR,sender1,sender2,reciever);
				return;
			}
			case 6://add XOR structure
			{
				int sender1 = std::rand() % size;
                                while(!(index[sender1 / 32] & 1<<(sender1 % 32)))
                                        sender1 = std::rand() % size;
                                int sender2 = std::rand() % size;
                                while(!(index[sender2 / 32] & 1<<(sender2 % 32)))
                                        sender2 = std::rand() % size;
                                int reciever = std::rand() % size;
                                while(!(index[reciever / 32] & 1<<(reciever % 32)))
                                        reciever = std::rand() % size;
                                (*this).addStructure(XOR,sender1,sender2,reciever);
				return;
			}
			case 7://add NOT structure
			{
				int sender1 = std::rand() % size;
                                while(!(index[sender1 / 32] & 1<<(sender1 % 32)))
                                        sender1 = std::rand() % size;
                                int reciever = std::rand() % size;
                                while(!(index[reciever / 32] & 1<<(reciever % 32)))
                                        reciever = std::rand() % size;
                                (*this).addStructure(NOT,sender1,0,reciever);
				return;
			}
			case 8://add POR structure
			{
                                int sender1 = std::rand() % size;
                                while(!(index[sender1 / 32] & 1<<(sender1 % 32)))
                                        sender1 = std::rand() % size;
                                int reciever = std::rand() % size;
                                while(!(index[reciever / 32] & 1<<(reciever % 32)))
                                        reciever = std::rand() % size;
                                (*this).addStructure(POR,sender1,0,reciever);
				return;
                        }
			case 9://add PAND structure
			{
                                int sender1 = std::rand() % size;
                                while(!(index[sender1 / 32] & 1<<(sender1 % 32)))
                                        sender1 = std::rand() % size;
                                int reciever = std::rand() % size;
                                while(!(index[reciever / 32] & 1<<(reciever % 32)))
                                        reciever = std::rand() % size;
                                (*this).addStructure(PAND,sender1,0,reciever);
				return;
                        }
			case 10://add reciever
			{
				int sender = std::rand() % size;
                                while(!(index[sender / 32] & 1<<(sender % 32)))
                                        sender = std::rand() % size;
                                int reciever = std::rand() % size;
                                while(!(index[reciever / 32] & 1<<(reciever % 32)))
                                        reciever = std::rand() % size;
                                neurons[sender].addReciever(reciever);
                                return;
			}
			case 11://remove reciever
			{
				int target = std::rand() % size;
				while(!(index[target / 32] & 1<<(target % 32)))
                                        target = std::rand() % size;
				int rsize = 0;
				if(neurons[target].recievers.empty())
					return;
				for(auto it = neurons[target].recievers.begin(); it != neurons[target].recievers.end(); ++it){
					rsize++;
				}
				int remove = std::rand() % rsize;
				auto it = neurons[target].recievers.begin();
				for(int i = 0;i < remove;i++)
					++it;
				neurons[target].removeReciever(*it);		
				return;	
			}
			case 12://extend inputs processing time by 2
			{
				for(auto it = inputs.begin();it != inputs.end();++it){
					std::vector<int> input = nextLocations(2);
					if(input.empty())
						return;
					Neuron n1 = neurons[*it];
					n1.criticalCharge = 2;
					(*this).addNeuron(n1);
					Neuron n2 = Neuron();
					n2.addReciever(input[0]);
					(*this).addNeuron(n2);
					neurons[*it].pulse = 2;
					neurons[*it].recievers = std::forward_list<int>();
					neurons[*it].addReciever(input[1]);
				}
			}
		}		
	
}

Network& Network::operator=(const Network &i){
        
	if(this != &i){
		if(neurons != nullptr){
	                delete[] neurons;
	        }
	        if(index != nullptr){
	                delete[] index;
	        }
		size = i.size;
		neurons = new Neuron[size];
		index = new int[size / 32];
			
		for(int j = 0;j < size / 32;j++){
			index[j] = i.index[j];
		}
		for(int j = 0;j < size;j++){
			neurons[j] = i.neurons[j];
		}
		
		inputs = i.inputs;
		outputs = i.outputs;
		available = i.available;
        }
        return *this;
}

Network::~Network(){
	if(neurons != nullptr){
		delete[] neurons;
	}
	if(index != nullptr){
		delete[] index;
	}	
}

int Network::expendable(int e){
	for(auto it = inputs.begin();it != inputs.end();++it)
        	if(*it == e) 
	                return 0;
        for(auto it = outputs.begin();it != outputs.end();++it)
                if(*it == e)
                        return 0;
	return 1;
}

int Network::anyExpendable(){
	for(int i = 0;i < size;i++){
		if((index[i / 32] & 1<<(i % 32)) && expendable(i))
			return 1;
	}
	return 0;
}

int Network::nextLocation(){
	if(size == 0)
		return 0;
	if(!available.empty())
		return available.top();
	if(size < MAXSIZE){
		return size;
	}
	return -1;
}

std::vector<int> Network::nextLocations(int locations){
	std::vector<int> result;
	if(locations > MAXSIZE)
		return result;
	if(size == 0){
		for(int i = 0;i < locations;i++){
			result.push_back(i);
		}
		return result;	
	}
	std::stack<int> temp;
	while(!available.empty()){
		temp.push(available.top());
		result.push_back(available.top());
		locations--;
		available.pop();
		if(locations == 0){
			while(!temp.empty()){
				available.push(temp.top());
				temp.pop();
			}
			return result;
		}
	}
	if(size + locations <= MAXSIZE){
                for(int i = 0;i < locations;i++){
			result.push_back(size + i);
		}
		return result;
        }
	result.clear();
        return result;
}

void Network::clear(){
	for(int j = 0;j < size;j++){
                if(index[j / 32] & 1<<(j % 32))
			neurons[j].charge = 0;
	}
}

void Network::outputTree(std::vector<int>& tree,int target){
	int bitmap[size/32];
	std::forward_list<int> data[size];
	for(int i = 0;i < size;i++){
                if(index[i / 32] & (1<<(i % 32))){
			for(auto it = neurons[i].recievers.begin();it != neurons[i].recievers.end();++it){
				data[*it].push_front(i);
			}
		}
	}
        for(int i = 0;i < size/32;i++)
                bitmap[i] = 0;
        recursiveOutputTree(bitmap, data, target);
        for(int i = 0;i < size;i++){
                if(bitmap[i / 32] & (1<<(i % 32))){
                        tree.push_back(i);
		}
        }
}

void Network::inputTree(std::vector<int>& tree, std::pair<std::forward_list<int>,int >& data){
	int bitmap[size/32];
	for(int i = 0;i < size/32;i++)
		bitmap[i] = 0;
	int i = 0;
	for(auto it = data.first.begin();it != data.first.end();++it){
		if(*it)
			if(index[i / 32] & (1<<(i % 32)) && !(bitmap[i / 32] & (1<<(i % 32))))
				recursiveInputTree(bitmap, data, i);
		
		++i;
	}
	for(int i = 0;i < size;i++){
		if(bitmap[i / 32] & (1<<(i % 32)))
			tree.push_back(i);
	}
}

void Network::recursiveInputTree(int* bitmap,std::pair<std::forward_list<int>, int >& data, int current){
	bitmap[current / 32] |= 1<<(current % 32);

	for(auto it = neurons[current].recievers.begin();it != neurons[current].recievers.end();++it){
		if(!(bitmap[*it / 32] & (1<<(*it % 32))))
			recursiveInputTree(bitmap, data, *it);
	}
}

void Network::recursiveOutputTree(int* bitmap,std::forward_list<int>* data, int current){
        bitmap[current / 32] |= 1<<(current % 32);

        for(auto it = data[current].begin();it != data[current].end();++it){
                if(!(bitmap[*it / 32] & (1<<(*it % 32))))
                        recursiveOutputTree(bitmap, data, *it);
        }
}
