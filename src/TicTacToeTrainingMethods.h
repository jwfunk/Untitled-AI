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
	static void staticTraining(Network&, std::vector<std::pair<std::forward_list<int>,int> > &);
	static int evaluate(Network&);
	static void staticConvert(std::forward_list<int>&,int *,int);

	static void dynamicConvert(std::forward_list<int>&,int);

	static int tevaluate(Network&);
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
	static int locate(std::vector<std::pair<std::forward_list<int>,int> > &,std::forward_list<int>);
	
	static int contains(std::vector<std::pair<std::forward_list<int>,int> > &,std::forward_list<int>);
	static void trainPrecisionLearning( Network &, std::vector<std::pair<std::forward_list<int>,int> > &);
};
#endif
