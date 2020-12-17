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

	Trainer() = default;
	Trainer(int i): objective(i) {}

	/**
	 *Trains the Network using the given evaluate and mutate functions
	 *
	 *@param end address of an int variable used to stop training
	 *@param n Network that will be trained
	 */
	void dynamicTraining(int* end,Network& n,int);

	virtual int mutate(Network&,int) = 0;
	virtual int evaluate(Network&,Network&) = 0;

	private:

	void dynamicTrainingThread(Network*,int*,std::condition_variable*,bool*,std::mutex*,int*,int**);
	
	int objective{1};

};
#endif
