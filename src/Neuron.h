//
//Neuron.h
//
//header file for Neuron class used in network
//Neurons contain:
//charge: current charge of Neuron
//criticalCharge: charge needed for current Neuron to pulse
//pulse: charge that current Neuron sends to its recievers
//recievers: indexes of Neurons in the network that current Neuron pulses

#ifndef NEURON
#define NEURON

#include <iostream>
#include <string>
#include <list>
class Neuron{

	friend class Trainer;
	friend class Network;
	friend std::ostream& operator<<(std::ostream&, const Neuron&);

	public:
	
	//constructors
	Neuron() = default;
	Neuron(int cc,int p): criticalCharge(cc), pulse(p) {}

	
	//Adds specified int to recievers list. Currently allows duplicates
	int addReciever(int);

	//Removes all instances of specified int from recievers list if it exists
	int removeReciever(int);
	
	//displays the current Neuron's info
	const std::string info() const;

	//copy assignment operator
	Neuron& operator=(const Neuron &);

	std::list<int>* getRecievers(){return &recievers;}

	int getPulse() {return pulse;}

	private:
	int charge{0};
	int criticalCharge{1};
	int pulse{1};
	std::list<int> recievers;

};

#endif
