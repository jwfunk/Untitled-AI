
//Neuron.cpp

#include "Neuron.h"
#include <iterator>

std::ostream& operator<<(std::ostream &os, const Neuron &i){
	os << i.info();
	return os;
}

const std::string Neuron::info() const {
	std::string r = "Charge: " + std::to_string(charge) + "\nCritical Charge: " + std::to_string(criticalCharge) + "\nPulse: " + std::to_string(pulse) + "\nRecievers: ";
	std::forward_list <int> :: iterator it;
        if(recievers != nullptr)
		for(it = recievers->begin(); it != recievers->end(); ++it){
			r += std::to_string(*it) + ",";
		}
	r += "-1";
	return r;
}

int Neuron::addReciever(int i) {
	if(recievers == nullptr)
		recievers = new std::forward_list<int>;
	recievers->push_front(i);
	return 0;
}

int Neuron::removeReciever(int i) {
	if(recievers != nullptr)
		recievers->remove(i);
	return 0;
}

Neuron& Neuron::operator=(const Neuron &i){
	if(this != &i){
		this->charge = i.charge;
		this->criticalCharge = i.criticalCharge;
		this->pulse = i.pulse;
		std::forward_list <int> :: iterator it;
		if(i.recievers != nullptr)
			for(it = i.recievers->begin(); it != i.recievers->end(); ++it)
				(*this).addReciever(*it);
	}
	return *this;
}
