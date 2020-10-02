
//Neuron.cpp

#include "Neuron.h"
#include <iterator>

std::ostream& operator<<(std::ostream &os, const Neuron &i){
	os << i.info();
	return os;
}

const std::string Neuron::info() const {
	std::string returnValue = "Charge: " + std::to_string(charge) + "\nCritical Charge: " + std::to_string(criticalCharge) + "\nPulse: " + std::to_string(pulse) + "\nRecievers: ";
        
	for(auto recieversIterator = recievers.begin(); recieversIterator != recievers.end(); ++recieversIterator){
		returnValue += std::to_string(*recieversIterator) + " ";
	}
	returnValue += "-1";
	return returnValue;
}

Neuron& Neuron::operator=(const Neuron &i){
	if(this != &i){
		this->charge = i.charge;
		this->criticalCharge = i.criticalCharge;
		this->pulse = i.pulse;
		this->recievers = i.recievers;
	}
	return *this;
}
