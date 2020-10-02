//
//Neuron.h
//
//header file for Neuron class used in network

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
	/**
	 *Constructs a Neuron with default values for criticalCharge and pulse
	 */
	Neuron() = default;

	/**
	 *Constructs a Neuron object with it's critical Charge and pulse initialized
	 *
	 *@param cc Value criticalCharge is initialized to
	 *@param p  Value pulse is initialized to
	 */
	Neuron(int cc,int p): criticalCharge(cc), pulse(p) {}

	
	/**
	 *Add an integer value to recievers indicating the index of a neuron that the current one
	 *connects to
	 *
	 * @param i Value of integer added
	 */
	void addReciever(int i) {recievers.push_back(i);}

	/**
	 *Removes all instances of i from recievers if they exits
	 *
	 * @param i Value of integer to remove
	 */
	void removeReciever(int i) {recievers.remove(i);}

	/**
	 *Returns the pulse of the neuron
	 *
	 *@return pulse of neuron
	 */
	int getPulse() {return pulse;}
	
	/**
	 *Returns a string describing the state of the neuron
	 */
	const std::string info() const;

	/**
	 *Performs a deep copy of the given neuron
	 */
	Neuron& operator=(const Neuron &);

	/**
	 *Returns an iterator at the beginning of recievers
	 *
	 *@return recievers.begin()
	 */
	std::list<int>::iterator recieversBegin() {return recievers.begin();}

	/**
	 *Returns an iterator at the end of recievers
	 *
	 *@return recievers.end()
	 */
	std::list<int>::iterator recieversEnd() {return recievers.end();}

	/**
	 *Returns 1 if recievers is empty 
	 *Returns 0 otherwise
	 *
	 *@return recievers.empty()
	 */
	int recieversEmpty() {return recievers.empty();}

	/**
	 *Returns the first element of recievers
	 *
	 *@return recievers.front()
	 */
	int recieversFront() {return recievers.front();}

	private:
	int charge{0};
	int criticalCharge{1};
	int pulse{1};
	std::list<int> recievers;

};

#endif
