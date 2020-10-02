//
//TicTacToeTrainingMethods.h
//

#ifndef TICTACTOETRAININGMETHODS
#define TICTACTOETRAININGMETHODS

#include "TicTacToeTrainingMethods.h"
#include "Network.h"
#include <string>
#include <forward_list>

class TicTacToeTrainingMethods{
	public:
	static int evaluate(Network&);

	static void dynamicConvert(std::forward_list<int>&,int);

	static std::string newDynamicTicTacToe(Network&);
	static int choose(Network&);
	static int mutate(Network&,int);
	static int recursiveMutate(Network&,int);

	static int noRecievers(Network&,int,int);

	static void recursiveAddReciever(Network&,int);
	static int randomLoc(std::forward_list<int>&);

	static void getInputsLeft(Network&,std::forward_list<int>&,int);
	static void getOutputsLeft(Network&,std::forward_list<int>&,int);

	static void clean(Network&);
};
#endif
