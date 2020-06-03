
//Network.cpp

#include "Network.h"
#include <iterator>
#include <cstdlib>
#include <ctime>

Network::Network(){
	inputs = new std::forward_list<int>;
	outputs = new std::forward_list<int>;
}

std::ostream& operator<<(std::ostream &os, const Network &i){

	os << i.info();
	return os;
}

const std::string Network::info() const {
	if(size == 0)
		return "Empty Network";
	std::string r = "Inputs: ";
	std::forward_list <int> :: iterator it;
	for(it = inputs->begin(); it != inputs->end(); ++it)
		r += std::to_string(*it) + ",";
	r += "-1\nOutputs: ";
	for(it = outputs->begin(); it != outputs->end(); ++it)
		r += std::to_string(*it) + ",";
	r += "-1\n";
	int e = 0;	
	for(int i = 0;i < size / 32;i++){
		for(int j = 0;j < 32;j++){
			if((index[i] & (1<<j))){
				r += "Neuron: " + std::to_string(j + (32 * i)) + "\n" + neurons[(i * 32) + j].info() + "\n";
				e = 1;	
			}
		}
	}
	if(e == 0)
		return "Empty Network";	
	return r;
}

int Network::nextLocation(){
	if(size == 0)
		return 0;
	for(int i = 0;i < size / 32;i++){
                if(index[i] != -1){
                        for(int j = 0;j < 32;j++){
                                if(!(index[i] & (1<<j))){//Free spot in bitmap
					return (i * 32) + j;
				}
			}
		}
	}
	if(size < MAXSIZE){
		return size;
	}
	return -1;
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
			Neuron input1 = Neuron(neurons[i1].pulse,3);
			Neuron input2 = Neuron(neurons[i2].pulse,3);
			int loc1 = (*this).nextLocation();
			if(loc1 == -1)
				return -1;
			neurons[i1].addReciever(loc1);
			(*this).addNeuron(input1);
			int loc2 = (*this).nextLocation();
			if(loc2 == -1){
				(*this).removeNeuron(loc1);
				return -1;
			}
			neurons[i2].addReciever(loc2);
			(*this).addNeuron(input2);
			Neuron n3 = Neuron(1,1);
			int loc3 = (*this).nextLocation();
			if(loc3 == -1){
				(*this).removeNeuron(loc1);
				(*this).removeNeuron(loc2);
				return -1;
			}
			neurons[loc1].addReciever(loc3);
			(*this).addNeuron(n3);
			int loc4 = (*this).nextLocation();
			if(loc4 == -1){
				(*this).removeNeuron(loc1);
                                (*this).removeNeuron(loc2);
				(*this).removeNeuron(loc3);
				return -1;
			}
			Neuron n4 = Neuron(1,1);
			neurons[loc2].addReciever(loc4);
			(*this).addNeuron(n4);
			Neuron n5 = Neuron(1,-3);
			int loc5 = (*this).nextLocation();
			if(loc5 == -1){
				(*this).removeNeuron(loc1);
                                (*this).removeNeuron(loc2);
                                (*this).removeNeuron(loc3);
				(*this).removeNeuron(loc4);
				return -1;
			}
			n5.addReciever(loc4);
			neurons[loc1].addReciever(loc5);
			(*this).addNeuron(n5);
			int loc6 = (*this).nextLocation();
			if(loc6 == -1){
				(*this).removeNeuron(loc1);
                                (*this).removeNeuron(loc2);
                                (*this).removeNeuron(loc3);
                                (*this).removeNeuron(loc4);
				(*this).removeNeuron(loc5);
				return -1;
			}
			Neuron n6 = Neuron(1,-3);
			n6.addReciever(loc3);
			neurons[loc2].addReciever(loc6);
			(*this).addNeuron(n6);
			int loc7 = (*this).nextLocation();
			if(loc7 == -1){
				(*this).removeNeuron(loc1);
                                (*this).removeNeuron(loc2);
                                (*this).removeNeuron(loc3);
                                (*this).removeNeuron(loc4);
                                (*this).removeNeuron(loc5);
				(*this).removeNeuron(loc6);
				return -1;
			}
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
		return 0;
	}
	for(int i = 0;i < size / 32;i++){
		if(index[i] != -1){
			for(int j = 0;j < 32;j++){
				if(!(index[i] & (1<<j))){//Free spot in bitmap
					neurons[(i * 32) + j] = n;
					index[i] |= 1<<j;
					return 0;
				}
			}
		}
	}
	if(size < MAXSIZE){
		size *= 2;
		int *newindex = new int[size / 32];
		Neuron *newneurons = new Neuron[size];
		for(int i = 0;i < size / 64;i++){
			newindex[i] = index[i];
			newindex[i + size / 64] = 0;
		}
		for(int i = 0;i < size / 2;i++)
			newneurons[i] = neurons[i];
		delete[] index;
		for(int j = 0;j < size / 2;j++){
                        if(neurons[j].recievers != nullptr){
                                //neurons[j].recievers->clear();
                                delete(neurons[j].recievers);
                        }
                }
		delete[] neurons;
		index = newindex;
		neurons = newneurons;
		this->addNeuron(n);
		return 0;
	}
	return 1;
}

int Network::removeNeuron(int i){
	if(i >= size || i < 0)
		return 1;
	if(index[i / 32] & 1<<(i % 32)){
		index[i / 32] &= ~(1<<(i % 32));
		delete(neurons[i].recievers);
		neurons[i].recievers = nullptr;
		inputs->remove(i);
		outputs->remove(i);
		for(int j = 0;j < size;j++){
			if(index[j / 32] & 1<<(j % 32))
				neurons[j].removeReciever(i);
		}
	}
	return 0;
	
}

int Network::addInput(int i){
	if(i >= size || i < 0)
                return 1;
	if(index[i / 32] & 1<<(i % 32)){
		inputs->push_front(i);
		return 0;
	}
	return 1;
}

int Network::removeInput(int i){
	inputs->remove(i);
	return 0;
}

int Network::addOutput(int i){
	if(i >= size || i < 0)
                return 1;
        if(index[i / 32] & 1<<(i % 32)){
                outputs->push_front(i);
		neurons[i].pulse = neurons[i].criticalCharge;
		neurons[i].addReciever(i);
                return 0;
        }
        return 1;
}

int Network::removeOutput(int i){
	outputs->remove(i);
	return 0;
}

int Network::process(std::forward_list<int> *given){

	//Activate input Neurons
	std::forward_list <int> :: iterator it1;
	std::forward_list <int> :: iterator it2;
	it2 = given->begin();
	for(it1 = inputs->begin(); it1 != inputs->end(); ++it1){
		if(it2 == given->end()){
			(*this).clear();//clear the network
			return -1;
		}
		if(*it2)
			neurons[*it1].charge = neurons[*it1].criticalCharge;
		++it2;
	}
	if(it2 != given->end()){
		(*this).clear();//clear the network

		return -1;
	}
	int changed;	
	//Process the Data
	for(int i = 0;i < MAXITERATIONS;i++){
		changed = 0;
		for(int j = 0;j < size;j++){
                        if(index[j / 32] & 1<<(j % 32))
                                if(neurons[j].charge >= neurons[j].criticalCharge){
					if(neurons[j].recievers != nullptr)
						for(it1 = neurons[j].recievers->begin();it1 != neurons[j].recievers->end();++it1){
							neurons[*it1].charge += neurons[j].pulse;
						}
					neurons[j].charge -= neurons[j].criticalCharge;
					changed = 1;
				}
                }

		for(it1 = outputs->begin();it1 != outputs->end();++it1)
			if(neurons[*it1].charge >= neurons[*it1].criticalCharge){
				(*this).clear();//clear network charges
				return *it1;
			}
		if(!changed){
			(*this).clear();
			return -1;
		}
	}
	(*this).clear();	
	return -1;
}

void Network::clear(){
	for(int j = 0;j < size;j++){
                        if(index[j / 32] & 1<<(j % 32))
				neurons[j].charge = 0;
	}
}

void Network::mutate(){
	for(int m = 0;m < NMUTATIONS;m++){
		
		int choice;
		int mutation = std::rand() % (4 * size + 7);
		if(mutation < size)
			choice = 2;
		if(mutation >= size && mutation < (2 * size))
			choice = 3;
		if(mutation >= (2 * size) && mutation < (3 * size))
			choice = 10;
		if(mutation >= (3 * size) && mutation < (4 * size))
			choice = 1;
		if(mutation == (4 * size))
			choice = 0;
		if(mutation == (4 * size + 1))
			choice = 4;
		if(mutation == (4 * size + 2))
                        choice = 5;
		if(mutation == (4 * size + 3))
                        choice = 6;
		if(mutation == (4 * size + 4))
                        choice = 7;
		if(mutation == (4 * size + 5))
                        choice = 8;
		if(mutation == (4 * size + 6))
                        choice = 9;
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
				delete(n.recievers);
				return;
			}
			case 1://remove Neuron
			{
				int remove = std::rand() % size;
				if(!anyExpendable())
					return;		
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
		}		
	}
}

Network& Network::operator=(const Network &i){
        
	if(this != &i){
		if(neurons != nullptr){
			for(int j = 0;j < size;j++){
				if(neurons[j].recievers != nullptr){
					neurons[j].recievers->clear();
					delete(neurons[j].recievers);
				}
			}
	                delete[] neurons;
	        }
	        if(index != nullptr){
	                delete[] index;
	        }
	        inputs->clear();
	        outputs->clear();
		size = i.size;
		neurons = new Neuron[size];
		index = new int[size / 32];
			
		for(int j = 0;j < size / 32;j++){
			index[j] = i.index[j];
		}
		for(int j = 0;j < size;j++){
			neurons[j] = i.neurons[j];
		}
		
		//inputs = new std::forward_list<int>;
        	//outputs = new std::forward_list<int>;               
		//for(auto it = i.inputs->begin();it != i.inputs->end();++it){
		//	inputs->push_front(*it);
		//} 
		//for(auto it = i.outputs->begin();it != i.outputs->end();++it){
		//	outputs->push_front(*it);
		//}
		*inputs = *(i.inputs);
		*outputs = *(i.outputs);
        }
        return *this;
}

Network::~Network(){
	if(neurons != nullptr){
		for(int j = 0;j < size;j++){
                        if(neurons[j].recievers != nullptr){
                                neurons[j].recievers->clear();
                                delete(neurons[j].recievers);
                        }
                }
		delete[] neurons;
	}
	if(index != nullptr){
		delete[] index;
	}
	inputs->clear();
	delete(inputs);
	outputs->clear();
	delete(outputs);
	
}

int Network::expendable(int e){
	for(auto it = inputs->begin();it != inputs->end();++it)
        	if(*it == e) 
	                return 0;
        for(auto it = outputs->begin();it != outputs->end();++it)
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
