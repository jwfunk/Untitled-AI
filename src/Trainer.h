//
//Trainer.h
//
//Header file for Trainer class
//Trainer is used to train a given network to output a better result based on the 
//training method.
//Training methods:
//Target Learning
//Reinforcement Learning

#ifndef TRAINER
#define TRAINER

#include "Network.h"
#include "Neuron.h"
#include <utility>
#include <iostream>
#include <vector>
#include <forward_list>

/*
//Precision Learning
//Given a vector of pairs
//pair.first is a forward list representing a vector of inputs for the network
//pair.second is the expected result for the input
//
//
//Below is a visual for the structures that are being used in the method
//
//
//
//   new case--O          O   Neurons that previously were affected by the inputs are set to 1 in bitmap
//          /  | \      / | \
//        /     |  \  /   |   \  
//      O       O    O    O    O  
//      |____________|         |
//      |     |                |
//      |   inputs             |
//      |______________________|
//                  |
//              contributers
*/

//Target Learning
//Given a vector of pairs
//pair.first is a forward list representing a vector of inputs for the network
//pair.second is the expected result for the input
//enumerations is the number of enumerations the training method will go through

class Trainer{

	public:

	static void dynamicTraining(Network&,int);

	static void staticTraining(Network&, std::vector<std::pair<std::forward_list<int>,int> > &);

	//Trains the network to play from moves given by the player
	//Always attains 100% accuracy but requires data to train from
	static void trainPrecisionLearning( Network &, std::vector<std::pair<std::forward_list<int>,int> > &);

	//Trains the network via random mutation
	//this has seen marginal success but seems largely
	//ineffective moving on
	static void trainTargetLearning(Network&,std::vector<std::pair<std::forward_list<int>,int> >&,int);

	static void staticConvert(std::forward_list<int>&,int *,int);

	static void dynamicConvert(std::forward_list<int>&,int);

	private:
	
	static int locate(std::vector<std::pair<std::forward_list<int>,int> > &,std::forward_list<int>);
	
	static int contains(std::vector<std::pair<std::forward_list<int>,int> > &,std::forward_list<int>);

};
#endif
