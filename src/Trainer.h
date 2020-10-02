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
#include "defs.h"
#include <utility>
#include <iostream>
#include <vector>
#include <forward_list>
#include <mutex>
#include <thread>
#include <condition_variable>


class Trainer{

	public:

	/**
	 *Constructs a Trainer with the given evaluate and mutate functions
	 *
	 *@param eval Address of the evaluate function
	 *@param mut  Address of the mutate function
	 */
	Trainer(int(*eval)(Network&),int(*mut)(Network&,int)):evaluate(eval),mutate(mut) {}

	/**
	 *Trains the Network using the given evaluate and mutate functions
	 *
	 *@param end address of an int variable used to stop training
	 *@param n Network that will be trained
	 */
	void dynamicTraining(int* end,Network& n);

	private:

	void dynamicTrainingThread(Network*,int*,std::condition_variable*,bool*,std::mutex*,int*,int**);

	int (*evaluate)(Network&);

	int (*mutate)(Network&,int);
};
#endif
