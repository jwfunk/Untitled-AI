//
//Network.h
//
//Header file for Network class
//Networks include:
//inputs: forward_list of all Neurons indexes considered inputs
//outputs: forward_list of all Neurons indexes considered outputs
//neurons: array of all neurons in the Network
//size: size of neurons array
//index: bitmap for empty positions in neurons
#ifndef NETWORK
#define NETWORK

#define INITSIZE 64
#define MAXSIZE 2048

#include "Neuron.h"
#include <forward_list>
#include <string>

class Network {

	friend std::ostream& operator<<(std::ostream&, const Network&);
	
	public:

	//constructors
	Network();

	//Adds the given neuron to the network. Returns 0 on success and 1 on failure
	int addNeuron(const Neuron);

	//Removes the Neuron at neurons[int]. Returns 0 on success and 1 on failure
	int removeNeuron(int);

	//Adds the int to inputs. Returns 0 on success and 1 on failure
	int addInput(int);

	//Removes the int from inputs. Returns 0 on success and 1 on failure
	int removeInput(int);

	const std::string info() const;

	private:
	
	std::forward_list<int> *inputs{nullptr};
	std::forward_list<int> *outputs{nullptr};
	Neuron *neurons{nullptr};
	int size{0};
	int *index{nullptr};
};

#endif
