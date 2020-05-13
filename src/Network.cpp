
//Network.cpp

#include "Network.h"

std::ostream& operator<<(std::ostream &os, const Network &i){

	os << i.info();
	return os;
}

const std::string Network::info() const {
	if(size == 0)
		return "Empty Network";
	std::string r = "";
	for(int i = 0;i < size / 32;i++){
		for(int j = 0;j < 32;j++){
			if((index[i] & (1<<j)))
				r += neurons[(i * 32) + j].info() + "\n";
		}
	}	
	return r;
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
		delete(index);
		delete(neurons);
		index = newindex;
		neurons = newneurons;
		this->addNeuron(n);
		return 0;
	}
	return 1;
}
