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
#include <forward_list>
class Neuron{

	friend class Network;
	friend std::ostream& operator<<(std::ostream&, const Neuron&);

	public:
	
	//constructors
	Neuron() = default;
	Neuron(int cc,int p): criticalCharge(cc), pulse(p) {}
	
	//displays the current Neuron's info
	const std::string info() const;

	Neuron& operator=(const Neuron &);

	private:
	int charge{0};
	int criticalCharge{1};
	int pulse{1};
	std::forward_list<int> *recievers{nullptr};

};

#endif
