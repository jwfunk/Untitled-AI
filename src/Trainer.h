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
#include <mutex>
#include <thread>
#include <condition_variable>

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

class Trainer{

	public:

	Trainer(int(*eval)(Network&),int(*mut)(Network&,int)):evaluate(eval),mutate(mut) {}

	void dynamicTraining(int*,Network&,int);


	//Trains the network to play from moves given by the player
	//Always attains 100% accuracy but requires data to train from

	private:

	void dynamicTrainingThread(Network*,int*,std::condition_variable*,bool*,std::mutex*,int*,int**);
	

	int (*evaluate)(Network&);

	int (*mutate)(Network&,int);
};
#endif
