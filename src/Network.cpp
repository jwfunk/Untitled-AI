//Network.cpp

#include "Network.h"
#include <iterator>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <fstream>
#include "Exceptions.h"

Network::Network(){
	size = 0;
}

Network::Network(const Network &i){
	if(neurons != nullptr){
		delete[] neurons;
	}
	if(index != nullptr){
		delete[] index;
	}
	size = i.size;
	neurons = new Neuron[size];
	index = new int[size / 32];

	for(int j = 0;j < size / 32;++j){
		index[j] = i.index[j];
	}
	for(int j = 0;j < size;++j){
		neurons[j] = i.neurons[j];
	}

	inputs = i.inputs;
	outputs = i.outputs;
	available = i.available;
	dynamic = i.dynamic;
	senders = i.senders;
	recievers = i.recievers;
	notTargets = i.notTargets;
	retroactive = i.retroactive;

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
		returnValue += "Not Targets: ";
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
	for(int i = 0;i < size;++i){
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
        for(int i = 0;i < INITSIZE / 32;++i)
        	index[i] = 0;
        size = INITSIZE;
        available = std::stack<int>();
	while(!fs.eof()){
		if(buffer != "Neuron:")
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
		std::list<int> rec;
		fs >> buffer;
		fs >> i;
		while(i != -1){
			rec.push_back(i);
			fs >> i;
		}
		if(loc >= size){
			int oldSize = size;
			while(loc >= size)
				size *= 2;
			int *newindex = new int[size / 32];
                        Neuron *newneurons = new Neuron[size];
                        for(int i = 0;i < oldSize;++i)
                                newneurons[i] = neurons[i];
                        for(int i = 0;i < oldSize / 32;++i){
                                newindex[i] = index[i]; 
                        }
			for(int i = oldSize / 32;i < size / 32;++i){
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
		neurons[loc].recievers = rec;
		index[loc / 32] |= 1<<(loc % 32);
		fs >> buffer;
	}
	for(int i = size - 1;i >= 0;--i)
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
			senders.push_front(loc);
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
			senders.push_front(loc);
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
			senders.push_front(loc7);
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
		case TIMED:
		{
			std::vector<int> locs = (*this).nextLocations(3);
			if(locs.empty())
				return -1;
			(*this).neurons[i1].addReciever(locs[0]);
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
			n3.addReciever(i3);
			(*this).addNeuron(n3);
			recievers.push_front(locs[0]);
			notTargets.push_front(locs[2]);
			senders.push_front(locs[2]);

		}
		default:
		{
			break;
		}
	}
	return -1;
}

Neuron Network::getNeuron(int i){
	if(i < 0 || i > size)
		throw invalidBoundsException();
	if(!(index[i / 32] & 1<<(i % 32)))
		throw invalidIndexException();
	return neurons[i];	
}

void Network::setNeuron(Neuron n,int i){
	if(i < 0 || i > size)
		throw invalidBoundsException();
	if(!(index[i / 32] & 1<<(i % 32)))
		throw invalidIndexException();
	for(auto recieversIterator = n.recievers.begin(); recieversIterator != n.recievers.end();++recieversIterator){
		if(!(index[*recieversIterator / 32] & 1<<(*recieversIterator % 32)))
			throw badRecieverException();
	}
	neurons[i] = n;
}

void Network::addNeuron(const Neuron n){
	for(auto recieversIterator = n.recievers.begin();recieversIterator != n.recievers.end();++recieversIterator){
		if(!(index[*recieversIterator / 32] & 1<<(*recieversIterator % 32)))
			throw badRecieverException();
			
	}
	if(size == 0){
		neurons = new Neuron[INITSIZE];
		neurons[0] = n;
		index = new int[INITSIZE / 32];
		for(int i = 0;i < INITSIZE / 32;++i)
			index[i] = 0;
		index[0] = 1;
		size = INITSIZE;
		available = std::stack<int>();
		for(int i = INITSIZE - 1;i > 0;--i){
			available.push(i);
		}
		return;
	}
	if(!available.empty()){
		neurons[available.top()] = n;
		index[available.top() / 32] |= 1<<(available.top() % 32);
		available.pop();
	}
	else{
		if(size < MAXSIZE){
			size *= 2;
			int *newindex = new int[size / 32];
			Neuron *newneurons = new Neuron[size];
			for(int i = 0;i < size / 2;++i)
                        	newneurons[i] = neurons[i];
			for(int i = 0;i < size / 64;++i){
                	        newindex[i] = index[i];
                	        newindex[i + size / 64] = 0;
                	}
			delete[] neurons;
			delete[] index;
			neurons = newneurons;
			index = newindex;
			for(int i = 0;i < size / 2;++i){
				available.push(size - i - 1);
			}
                	this->addNeuron(n);
		}
	}
}

void Network::removeNeuron(int i){
	if(i < 0 || i > size)
		throw invalidBoundsException();
	if(!(index[i / 32] & 1<<(i % 32)))
		throw invalidIndexException();
	index[i / 32] &= ~(1<<(i % 32));
	inputs.remove(i);
	outputs.remove(i);
	senders.remove(i);
	recievers.remove(i);
	notTargets.remove(i);
	for(int j = 0;j < size;++j){
		if(index[j / 32] & 1<<(j % 32))
			neurons[j].removeReciever(i);
	}
	available.push(i);
}

void Network::addInput(int i){
	if(i < 0 || i > size)
		throw invalidBoundsException();
	if(!(index[i / 32] & 1<<(i % 32)))
		throw invalidIndexException();
	inputs.push_front(i);
}

void Network::removeInput(int i){
	if(i < 0 || i > size)
		throw invalidBoundsException();
	if(!(index[i / 32] & 1<<(i % 32)))
		throw invalidIndexException();
	inputs.remove(i);
}

void Network::addOutput(int i){
	if(i < 0 || i > size)
		throw invalidBoundsException();
	if(!(index[i / 32] & 1<<(i % 32)))
		throw invalidIndexException();
	outputs.push_front(i);
}

void Network::removeOutput(int i){
	if(i < 0 || i > size)
		throw invalidBoundsException();
	if(!(index[i / 32] & 1<<(i % 32)))
		throw invalidIndexException();
	outputs.remove(i);
}

int Network::process(std::forward_list<int> *given){

	//Activate input Neurons
	
	std::queue <int> active;
	std::queue <int> debug;
	active = retroactive;
	retroactive = std::queue<int>();
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
	if(givenIterator != given->end()){//should have no effect on a dynamic network, check length before changing charges
		(*this).clear();//clear the network
		return -1;
	}
	active.push(-1);
	int iterations = 0;

	//process the data
	
	while(!active.empty()){
		int i = active.front();
		if(!(index[i / 32] & 1<<(i % 32)) || i < -1 || i > size)
			throw corruptedNetworkException();
		active.pop();
		if(i == -1){
			while(!active.empty() && active.front() == -1){
				active.pop();
			}
			for(auto outputsIterator = outputs.begin();outputsIterator != outputs.end();++outputsIterator)//possible improvement here
                         	if(neurons[*outputsIterator].charge >= neurons[*outputsIterator].criticalCharge){
                                 	if(!dynamic)
						(*this).clear();//clear network charges
					else{
						retroactive = active;
						neurons[*outputsIterator].charge -= neurons[*outputsIterator].criticalCharge;
					}
                                 	return *outputsIterator;
                         	}
			if(active.empty()){
				if(!dynamic)
					(*this).clear();	
				return -1;
			}
			active.push(-1);
		}
		else{
			if(neurons[i].charge >= neurons[i].criticalCharge){
				for(auto recieversIterator = neurons[i].recievers.begin();recieversIterator != neurons[i].recievers.end();++recieversIterator){
                        	        neurons[*recieversIterator].charge += neurons[i].pulse;
					active.push(*recieversIterator);
               	        	}
               	        	neurons[i].charge -= neurons[i].criticalCharge;
				if(neurons[i].charge >= neurons[i].criticalCharge){
					active.push(i);
				}
			}
		}
		
		if(iterations > MAXITERATIONS){
			if(!dynamic)
				(*this).clear();
        		return -1;
		}
		++iterations;
		
	}
	if(!dynamic)
		(*this).clear();	
	return -1;
}



Network& Network::operator=(const Network &i){
	if(neurons != nullptr){
		delete[] neurons;
	}
	if(index != nullptr){
		delete[] index;
	}
        
	if(this != &i){
		size = i.size;
		neurons = new Neuron[size];
		index = new int[size / 32];
			
		for(int j = 0;j < size / 32;++j){
			index[j] = i.index[j];
		}
		for(int j = 0;j < size;++j){
			neurons[j] = i.neurons[j];
		}
		
		inputs = i.inputs;
		outputs = i.outputs;
		available = i.available;
		dynamic = i.dynamic;
		senders = i.senders;
		recievers = i.recievers;
		notTargets = i.notTargets;
		retroactive = i.retroactive;
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
	for(int i = 0;i < size;++i){
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
		for(int i = 0;i < locations;++i){
			result.push_back(i);
		}
		return result;	
	}
	std::stack<int> temp;
	while(!available.empty()){
		temp.push(available.top());
		result.push_back(available.top());
		--locations;
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
                for(int i = 0;i < locations;++i){
			result.push_back(size + i);
		}
		return result;
        }
	result.clear();
        return result;
}

void Network::clear(){
	for(int j = 0;j < size;++j){
		neurons[j].charge = 0;
	}
	retroactive = std::queue<int>();
}

std::vector<int> Network::getInputs(int target){
	std::vector<int> inputs;
	for(int i = 0;i < size;++i){
                if(index[i / 32] & (1<<(i % 32))){
			for(auto recieversIterator = neurons[i].recievers.begin();recieversIterator != neurons[i].recievers.end();++recieversIterator){
                                if(*recieversIterator == target)
					inputs.push_back(i);
                        }
		}
	}
	return inputs;
}

int Network::randomSender(){
	int senderSize = 0;
	for(auto senderIterator = senders.begin();senderIterator != senders.end();++senderIterator)
		++senderSize;
	int choice = std::rand() % senderSize;
	auto senderIterator = senders.begin();
	for(int i = 0;i < choice;++i)
		++senderIterator;
	return *senderIterator;
}

int Network::randomReciever(){
	return *(recievers.begin());
}

int Network::randomNotTarget(){
        int notTargetSize = 0;
        for(auto notTargetIterator = notTargets.begin();notTargetIterator != notTargets.end();++notTargetIterator)
                ++notTargetSize;
        int choice = std::rand() % notTargetSize;
        auto notTargetIterator = notTargets.begin();
        for(int i = 0;i < choice;++i)
                ++notTargetIterator;
        return *notTargetIterator;
}


int Network::insert(int sender){
	int targetsSize = 0;
        for(auto recieversIterator = neurons[sender].recievers.begin();recieversIterator != neurons[sender].recievers.end();++recieversIterator)
        	++targetsSize;
        int target = std::rand() % targetsSize;
        auto recieversIterator = neurons[sender].recievers.begin();
        for(int i = 0;i < target;++i)
        	++recieversIterator;
        target = *recieversIterator;
	neurons[sender].removeReciever(target);
	return target;
}
