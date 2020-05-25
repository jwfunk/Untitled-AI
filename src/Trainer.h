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
#include "Neurons.h"
#include <utility>
#include <iostream>
#include <vector>
#include <forward_list>

//Target Learing
//Given a vector of pairs
//pair.first is a forward list representing a vector of inputs for the network
//pair.second is the expected result for the input
//enum is the number of enumerations the training method will go through
void trainTargetLearning(std::vector<std::pair<std::forward_list<int>,int> >,int);

#endif
