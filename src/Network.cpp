
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
	std::string returnValue = "Dynamic: ";

	returnValue += std::to_string(dynamic);

	returnValue += "\n";

	if(dynamic){
		returnValue += "Senders: ";
		for(auto sendersIterator = senders.begin();sendersIterator != senders.end();++sendersIterator)
			returnValue += std::to_string(*sendersIterator) + " ";
		returnValue += "-1\n";
		returnValue += "Recievers: ";
		for(auto recieversIterator = recievers.begin();recieversIterator != recievers.end();++recieversIterator)
			returnValue += std::to_string(*recieversIterator) + " ";
		returnValue += "-1\n";
		returnValue += "Not Targets : ";
		for(auto notTargetsIterator = notTargets.begin();notTargetsIterator != notTargets.end();++notTargetsIterator)
			returnValue += std::to_string(*notTargetsIterator) + " ";
		returnValue += "-1\n";
	}

	returnValue += "Inputs: ";
	
	for(auto inputsIterator = inputs.begin(); inputsIterator != inputs.end(); ++inputsIterator)
		returnValue += std::to_string(*inputsIterator) + " ";
	returnValue += "-1\nOutputs: ";
	for(auto outputsIterator = outputs.begin(); outputsIterator != outputs.end(); ++outputsIterator)
		returnValue += std::to_string(*outputsIterator) + " ";
	returnValue += "-1\n";
	int empty = 0;	
	for(int i = 0;i < size;i++){
		if((index[i / 32] & (1<<(i % 32)))){
			returnValue += "Neuron: " + std::to_string(i) + "\n" + neurons[i].info() + "\n";
			empty = 1;	
		}
	}
	if(empty == 0)
		return "Empty Network";	
	return returnValue;
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
	if(!fs.good())
		return -1;
	std::string buffer;
	fs >> buffer;
	fs >> dynamic;
	int i;
	std::forward_list<int> sen;
	std::forward_list<int> rec;
	std::forward_list<int> notT;
	if(dynamic){
		fs >> buffer;
		fs >> i;
		while(i != -1){
			sen.push_front(i);
			fs >> i;
		}
		fs >> buffer;
		fs >> i;
		while(i != -1){
			rec.push_front(i);
			fs >> i;
		}
		fs >> buffer;
		fs >> i;
		while(i != -1){
			notT.push_front(i);
			fs >> i;
		}
	}
	fs >> buffer;
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
		rec.clear();
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
                        for(int i = 0;i < oldSize / 32;i++){
                                newindex[i] = index[i]; 
                        }
			for(int i = oldSize / 32;i < size / 32;i++){
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
	for(auto inIterator = in.begin();inIterator != in.end();++inIterator)
		inputs.push_front(*inIterator);
	for(auto ouIterator = ou.begin();ouIterator != ou.end();++ouIterator)
                outputs.push_front(*ouIterator);
	for(auto senIterator = sen.begin();senIterator != sen.end();++senIterator)
		senders.push_front(*senIterator);
	for(auto recIterator = rec.begin();recIterator != rec.end();++recIterator)
		recievers.push_front(*recIterator);
	for(auto notTIterator = notT.begin();notTIterator != notT.end();++notTIterator)
		notTargets.push_front(*notTIterator);
	
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
			recievers.push_front(loc);
			notTargets.push_front(loc);
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
			recievers.push_front(loc);
                        notTargets.push_front(loc);
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
			recievers.push_front(loc);
			return 0;
		}
		case XOR://if i1 or i2 pulses alone i3 is pulsed
		{
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
			
			recievers.push_front(loc1);
			recievers.push_front(loc2);
			notTargets.push_front(loc7);
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
		senders.remove(i);
		recievers.remove(i);
		notTargets.remove(i);
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
		senders.push_front(i);
		return 0;
	}
	return 1;
}

int Network::removeInput(int i){
	inputs.remove(i);
	senders.remove(i);
	return 0;
}

int Network::addOutput(int i){
	if(i >= size || i < 0)
                return 1;
        if(index[i / 32] & 1<<(i % 32)){
                outputs.push_front(i);
		recievers.push_front(i);
		notTargets.push_front(i);
                return 0;
        }
        return 1;
}

int Network::removeOutput(int i){
	outputs.remove(i);
	recievers.remove(i);
	return 0;
}

int Network::process(std::forward_list<int> *given){

	//Activate input Neurons
	
	std::queue <int> active;
	active = retroactive;
	auto givenIterator = given->begin();
	for(auto inputsIterator = inputs.begin(); inputsIterator != inputs.end(); ++inputsIterator){
		if(givenIterator == given->end()){
			(*this).clear();//clear the network
			return -1;
		}
		if(*givenIterator){
			neurons[*inputsIterator].charge = neurons[*inputsIterator].criticalCharge;
			active.push(*inputsIterator);
		}
		++givenIterator;
	}
	if(givenIterator != given->end()){
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
			for(auto outputsIterator = outputs.begin();outputsIterator != outputs.end();++outputsIterator)//possible improvement here
                         	if(neurons[*outputsIterator].charge >= neurons[*outputsIterator].criticalCharge){
                                 	if(!dynamic)
						(*this).clear();//clear network charges
					else
						retroactive = active;
                                 	return *outputsIterator;
                         	}
		}
		else{
			if(neurons[i].charge >= neurons[i].criticalCharge){
				for(auto recieversIterator = neurons[i].recievers.begin();recieversIterator != neurons[i].recievers.end();++recieversIterator){
                        	        neurons[*recieversIterator].charge += neurons[i].pulse;
					active.push(*recieversIterator);
               	        	}
               	        	neurons[i].charge -= neurons[i].criticalCharge;
				if(neurons[i].charge >= neurons[i].criticalCharge)
					active.push(i);
				active.push(-1);
			}
		}
		
		if(iterations > MAXITERATIONS){
			if(!dynamic)
				(*this).clear();
        		return -1;
		}
		iterations++;
		
	}
	if(!dynamic)
		(*this).clear();	
	return -1;
}

void Network::mutateTarget(std::pair<std::forward_list<int>, int >& target){
	std::vector<int> iTree;
	std::vector<int> oTree;
	iTree.clear();
	oTree.clear();
	inputTree(iTree,target.first);
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

int Network::choose(){
	int numChoices = 9;
	int choices[] = {10,10,10,10,10,10,10,10,10};
	if(size == MAXSIZE && available.empty()){
		choices[0] = 0;
		choices[2] = 0;
		choices[3] = 0;
		choices[4] = 0;
		choices[5] = 0;
	}
	int randSize = 0;
	for(int i = 0;i < numChoices;i++)
		randSize += choices[i];
	int choice = std::rand() % randSize;
	for(int i = 0;i < numChoices;i++){
		choice -= choices[i];
		if(choice < 0)
			return i;
	}	
	return -1;
}

void Network::mutate(int choice = -1){
	std::forward_list<int> usedLocs;		
	for(int i = 0;i < NMUTATIONS;i++){
		choice = choose();
		switch(choice){
			case 0://Timed Structure
			{
				//eventually move to structure method
				std::vector<int> locs = (*this).nextLocations(3);
                                if(locs.empty())
                                        return;

				int target, sender;
				sender = randomSender();
				if(!neurons[sender].recievers.empty() && std::rand() % 2)//insert
					target = insert(sender);
				else//add
					target = randomReciever();

				(*this).neurons[sender].addReciever(locs[0]);
				Neuron n1 = Neuron();
                        	n1.addReciever(locs[1]);
                        	n1.criticalCharge = 1;
                        	n1.pulse = 4;
                        	(*this).addNeuron(n1);
                        	Neuron n2 = Neuron();
                        	n2.addReciever(locs[2]);
                        	(*this).addNeuron(n2);
                        	Neuron n3 = Neuron();
                        	n3.criticalCharge = 4;
                        	n3.addReciever(target);
                        	(*this).addNeuron(n3);
				usedLocs.push_front(sender);//possibly remove
				usedLocs.push_front(target);//possibly remove				
				recievers.push_front(locs[0]);
				notTargets.push_front(locs[2]);

				if(std::rand() % 2){
					Neuron n4 = Neuron();
					int loc = (*this).nextLocation();
					if(loc == -1)
						return;
					neurons[locs[0]].addReciever(loc);
					(*this).addNeuron(n4);
					senders.push_front(loc);
					notTargets.push_front(loc);
				}
				break;
			}
			case 1://remove Neuron
			{
				if(!anyExpendable())
					break;	
				int remove = std::rand() % size;
				while(!(index[remove / 32] & 1<<(remove % 32)) || !expendable(remove)){
                                        remove = std::rand() % size;
				}
				(*this).removeNeuron(remove);
				break;
			}
			case 2://add AND structure
			{
				int sender1 = randomSender();
				int sender2 = randomSender();
				int reciever = randomReciever();
				(*this).addStructure(AND,sender1,sender2,reciever);
				usedLocs.push_front(sender1);//
				usedLocs.push_front(sender2);//
				usedLocs.push_front(reciever);//
				break;
			}
			case 3://add OR structure
			{
				int sender1 = randomSender();
                                int sender2 = randomSender();
                                int reciever = randomReciever();
                                (*this).addStructure(OR,sender1,sender2,reciever);
				usedLocs.push_front(sender1);
				usedLocs.push_front(sender2);
				usedLocs.push_front(reciever);
				break;
			}
			case 4://add XOR structure
			{
				int sender1 = randomLoc(usedLocs);
                                int sender2 = randomLoc(usedLocs);
                                int reciever = randomLoc(usedLocs);
                                (*this).addStructure(XOR,sender1,sender2,reciever);
				usedLocs.push_front(sender1);
				usedLocs.push_front(sender2);
				usedLocs.push_front(reciever);
				break;
			}
			case 5://add NOT structure
			{
				int sender1 = randomSender();
                                int reciever = randomNotTarget();
                                (*this).addStructure(NOT,sender1,0,reciever);
				usedLocs.push_front(sender1);
				usedLocs.push_front(reciever);
				break;
			}
			case 6://add POR structure
			{
                                int sender = randomSender();
                                int reciever;
				if(!neurons[sender].recievers.empty() && std::rand() % 2)//insert
					reciever = insert(sender);
				else
					reciever = randomReciever();
                                (*this).addStructure(POR,sender,0,reciever);
				usedLocs.push_front(sender);
				usedLocs.push_front(reciever);
				break;
                        }
			case 7://add PAND structure
			{
				int sender = randomSender();
                                int reciever;
                                if(!neurons[sender].recievers.empty() && std::rand() % 2)//insert
                                        reciever = insert(sender);
                                else
                                        reciever = randomReciever();
                                (*this).addStructure(PAND,sender,0,reciever);
				usedLocs.push_front(sender);
				usedLocs.push_front(reciever);
                                break;
                        }
			case 8://reciever
			{
				int loc = randomSender();
				int choice = std::rand() % 3;
				switch(choice)
				{
					case 0://add
					neurons[loc].addReciever(randomReciever());	
					break;

					case 1://remove
					if(!neurons[loc].recievers.empty()){
						insert(loc);//possibly rename to convey why it's used here
					}
					break;
		
					case 2://replace
					if(!neurons[loc].recievers.empty()){
                                                insert(loc);//possibly rename to convey why it's used here
						neurons[loc].addReciever(randomReciever());
                                        }
					break;		
				}
			}
		}
	usedLocs.sort();
	usedLocs.unique();					
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
		dynamic = i.dynamic;
		senders = i.senders;
		recievers = i.recievers;
		notTargets = i.notTargets;
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
	for(auto inputsIterator = inputs.begin();inputsIterator != inputs.end();++inputsIterator)
        	if(*inputsIterator == e) 
	                return 0;
        for(auto outputsIterator = outputs.begin();outputsIterator != outputs.end();++outputsIterator)
                if(*outputsIterator == e)
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
	while(!temp.empty()){
                available.push(temp.top());
                temp.pop();
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
	retroactive = std::queue<int>();
}

void Network::outputTree(std::vector<int>& tree,int target){
	int bitmap[size/32];
	std::forward_list<int> data[size];
	for(int i = 0;i < size;i++){
                if(index[i / 32] & (1<<(i % 32))){
			for(auto recieversIterator = neurons[i].recievers.begin();recieversIterator != neurons[i].recievers.end();++recieversIterator){
				data[*recieversIterator].push_front(i);
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

void Network::inputTree(std::vector<int>& tree, std::forward_list<int>& data){
	int bitmap[size/32];
	for(int i = 0;i < size/32;i++)
		bitmap[i] = 0;
	int i = 0;
	for(auto dataIterator = data.begin();dataIterator != data.end();++dataIterator){
		if(*dataIterator)
			if(index[i / 32] & (1<<(i % 32)) && !(bitmap[i / 32] & (1<<(i % 32))))
				recursiveInputTree(bitmap, i);
		++i;
	}
	for(int i = 0;i < size;i++){
		if(bitmap[i / 32] & (1<<(i % 32)))
			tree.push_back(i);
	}
}

void Network::recursiveInputTree(int* bitmap, int current){
	bitmap[current / 32] |= 1<<(current % 32);

	for(auto recieversIterator = neurons[current].recievers.begin();recieversIterator != neurons[current].recievers.end();++recieversIterator){
		if(!(bitmap[*recieversIterator / 32] & (1<<(*recieversIterator % 32))))
			recursiveInputTree(bitmap, *recieversIterator);
	}
}

void Network::recursiveOutputTree(int* bitmap,std::forward_list<int>* data, int current){
        bitmap[current / 32] |= 1<<(current % 32);

        for(auto dataIterator = data[current].begin();dataIterator != data[current].end();++dataIterator){
                if(!(bitmap[*dataIterator / 32] & (1<<(*dataIterator % 32))))
                        recursiveOutputTree(bitmap, data, *dataIterator);
        }
}
int Network::randomSender(){
	int senderSize = 0;
	for(auto senderIterator = senders.begin();senderIterator != senders.end();++senderIterator)
		senderSize++;
	int choice = std::rand() % senderSize;
	auto senderIterator = senders.begin();
	for(int i = 0;i < choice;i++)
		++senderIterator;
	return *senderIterator;
}

int Network::randomReciever(){
        int recieverSize = 0;
        for(auto recieverIterator = recievers.begin();recieverIterator != recievers.end();++recieverIterator)
                recieverSize++;
        int choice = std::rand() % recieverSize;
        auto recieverIterator = recievers.begin();
        for(int i = 0;i < choice;i++)
                ++recieverIterator;
        return *recieverIterator;
}

int Network::randomNotTarget(){
        int notTargetSize = 0;
        for(auto notTargetIterator = notTargets.begin();notTargetIterator != notTargets.end();++notTargetIterator)
                notTargetSize++;
        int choice = std::rand() % notTargetSize;
        auto notTargetIterator = notTargets.begin();
        for(int i = 0;i < choice;i++)
                ++notTargetIterator;
        return *notTargetIterator;
}

int Network::randomLoc(std::forward_list<int>& usedLocs){
	if(usedLocs.empty()){
		int target = std::rand() % size; 
	        while(!(index[target / 32] & 1<<(target % 32)))
	        	target = std::rand() % size;
		return target;
	}
	else{
		int usedSize = 0;
		for(auto locsIterator = usedLocs.begin();locsIterator != usedLocs.end();++locsIterator)
			usedSize++;
		int choice = std::rand() % usedSize;
		if(choice - 3 < 0){//change integer value to affect percentage chance of reusing used location
			std::forward_list<int> empty;
			return randomLoc(empty);
		}
		else{
			choice = std::rand() % usedSize;
			auto usedIterator = usedLocs.begin();
			for(int i = 0;i < choice;i++)
				++usedIterator;
			return *usedIterator;
		}
	}
}

int Network::insert(int sender){
	int targetsSize = 0;
        for(auto recieversIterator = neurons[sender].recievers.begin();recieversIterator != neurons[sender].recievers.end();++recieversIterator)
        	targetsSize++;
        int target = std::rand() % targetsSize;
        auto recieversIterator = neurons[sender].recievers.begin();
        for(int i = 0;i < target;i++)
        	++recieversIterator;
        target = *recieversIterator;
	neurons[sender].removeReciever(target);
	return target;
}
