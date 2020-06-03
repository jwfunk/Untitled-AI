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

#define MAXITERATIONS 100
#define INITSIZE 64
#define MAXSIZE 2048
#define NMUTATIONS 20

#include "Neuron.h"
#include <forward_list>
#include <string>

enum structure {AND,OR,XOR,NOT,PAND,POR};
class Network {

	friend std::ostream& operator<<(std::ostream&, const Network&);
	
	public:

	//constructors
	Network();

	//Adds the given neuron to the network. Returns 0 on success and 1 on failure
	int addNeuron(const Neuron);

	//Removes the Neuron at neurons[int]. Returns 0 on success and 1 on failure
	int removeNeuron(int);

	//Adds logic gate structure specified by structure enum
	//first two ints are input Neurons to structure
	//Second int isn't used if unary structure
	//Third int is output Neuron of structure
	int addStructure(structure,int,int,int);

	//Adds the int to inputs. Returns 0 on success and 1 on failure. Can be duplicate inputs
	int addInput(int);

	//Removes the int from inputs. Returns 0 on success and 1 on failure
	int removeInput(int);

	//Adds the int to outputs. Returns 0 on success and 1 on failure
	int addOutput(int);

	//Removes the int from outputs. Returns 0 on success and 1 on failure
	int removeOutput(int);

	//given list must be the same length as inputs or returns -1
	//Activates input neurons if given list has a 1 for the specified index
	//ex:
	//inputs: 1 3 5 6
	//given:  0 1 1 0
	//Neurons 3 and 5 pulse in beginning
	//processes until an output neuron fires or maximum time hits
	//returns index of output neuron or -1 if max time is hit
	int process(std::forward_list<int>*);

	//Clears the charges of all Neurons in the network
	void clear();

	//mutates a random aspect of the Network
	void mutate();

	const std::string info() const;

	Network& operator=(const Network &);

	~Network();

	private:

	//returns the next free location in neurons or -1 if full
        int nextLocation();

	int expendable(int);

	int anyExpendable();
	
	std::forward_list<int> *inputs{nullptr};
	std::forward_list<int> *outputs{nullptr};
	Neuron *neurons{nullptr};
	int size{0};
	int *index{nullptr};
};

#endif
