//
//Network.h
//
//Header file for Network class
#ifndef NETWORK
#define NETWORK

#include "Neuron.h"
#include "defs.h"
#include <forward_list>
#include <string>
#include <vector>
#include <utility>
#include <stack>
#include <queue>

class Network {

	friend class Trainer;
	friend std::ostream& operator<<(std::ostream&, const Network&);
	
	public:

	/**
	 *Enums for the structures that can be added
	 *
	 *AND: Pulses when its two input Neurons pulse
	 *
	 *OR: Pulses when either of its input Neurons pulse
	 *
	 *XOR: Pulses when only one of its input Neurons pulse (Inputs must pulse within a certain time of each other)
	 *
	 *NOT: Prevents it's reciever from pulsing when its input Neuron pulses
	 *
	 *PAND: "Partial" AND, Can be used along with an AND structure to add more necessary inputs. For example instead of needing two input Neurons to pulse one could add a third
	 *
	 *POR: "Partial" OR, Works the same as PAND but with OR
	 *
	 *TIMED: Pulses to its recievers with a timed delay allowing a NOT structure to interupt it
	 */
	enum structure {AND,OR,XOR,NOT,PAND,POR,TIMED};

	//constructors

	/**
	 *Constructs a Network with no Neurons inputed
	 */
	Network();

	/**
	 *Constructs a Network with a deep copy of the given Network
	 */
	Network(const Network &);

	/**
	 *Sets the charges of all the Neurons in the Network to 0
	 */
	void clear();

	/**
	 *Adds the given Neuron to the Network
	 *
	 *@param n The Neuron being added to the Network
	 *@return 0 on success and 1 when Network is at max size
	 */
	void addNeuron(const Neuron);

	/**
	 *Removes the Neuron at the given index from the Network
	 *
	 *Removes the index from all other Neurons recievers in the Network
	 *
	 *@param i the index that's removed from the Network
	 */
	void removeNeuron(int i);

	/**
	 *Adds a structure to the network resembeling classic logic gates
	 *
	 *@param s structure enum indicating which kind of gate is added
	 *@param i1 index of first input Neuron to the structure
	 *@param i2 index of second input Neuron to the structure (only used with AND, OR, and XOR structures)
	 *@param i3 index of the recieving Neuron of the structure
	 *@see structure
	 */
	int addStructure(structure s,int i1,int i2,int i3);

	/**
	 *Adds the index to the inputs list if the Neuron at that index is valid
	 *
	 *@param i index added to inputs
	 */
	void addInput(int i);

	/**
	 *Removes the index from the inputs list if it exists
	 *
	 *@param i index removed from inputs
	 */
	void removeInput(int i);

	/**
	 *Adds the index to the outputs list if the Neuron at that index is valid
	 *
	 *@param i index added to outputs
	 */
	void addOutput(int i);

	/**
	 *Removes the index from the outputs list if it exists
	 *
	 *@param i index removed from outputs
	 */
	void removeOutput(int i);

	/**
	 *Recieves a list of 1's and 0's indicating whether or not the input Neuron at each index of the inputs list was activated
	 *
	 *Activated Neurons pulse to their recievers which in turn pulse to theirs if their critical Charge is reached
	 *
	 *This cycle is continued until an output Neuron pulses, all Neurons stop pulsing, or the max amount of pulses is reached
	 *
	 *@param Given List of 1's and 0's. Must be the same length as inputs list
	 *@return The index of the output neuron that pulses or -1
	 */
	int process(std::forward_list<int>*);

	/**
	 *@return Information of the Network organized in a string
	 */
	const std::string info() const;

	/**
	 *Performs a deep copy of the given Network
	 */
	Network& operator=(const Network &);

	/**
	 *Deallocates the memory reserved for the Network
	 */
	~Network();

	/**
	 *Saves the Network to the given file name
	 *
	 *@param file string containing the file name
	 */
	void save(std::string file);

	/**
	 *Loads the Network located in the given file
	 *
	 *@param file string containing the file name
	 */
	int load(std::string file);

	/**
	 *@return The size of the array holding the neurons
	 */
	int getSize() {return size;}

	/**
	 *@param i index to check 
	 *@return 1 if Neuron at index is valid 0 otherwise
	 */
	int getIndex(int i) {return index[i];}

	//TODO change to protect Validity of Network
	Neuron getNeuron(int i);
	
	void setNeuron(Neuron n, int i);

	//TODO remove these
	int randomSender();
	int randomReciever();
	int randomNotTarget();

	/**
	 *Returns a list of all the Neurons that pulse the Neuron at the specified index
	 *
	 *@param i Index to get the inputs of
	 */
	std::vector<int> getInputs(int i);

	//TODO raname these
	std::forward_list<int> senders;
	std::forward_list<int> recievers;
	std::forward_list<int> notTargets;

	/**
	 *@return The next available index or -1 if full
	 */
        int nextLocation();

	/**
	 *@param i The amount of free indexes to look for
	 *@return The next availabe i indexes or -1 if not enough space
	 */
	std::vector<int> nextLocations(int i);

	/**
	 *Variable used to keep track of whether or not this Network should be processed dynamically
	 *
	 *When dynamic is 0 the charges are cleared after processing
	 */
	int dynamic{0};

	private:

	//returns 1 if neuron at int is not an input or output
	//otherwise returns 0
	int expendable(int);

	//returns 1 if any neuron is not an input or output
	//0 otherwise
	int anyExpendable();

	int insert(int);

	std::queue<int> retroactive;

	std::stack<int> available;

	std::forward_list<int> inputs;

	std::forward_list<int> outputs;

	Neuron *neurons{nullptr};

	int size{0};

	int *index{nullptr};
};

#endif
