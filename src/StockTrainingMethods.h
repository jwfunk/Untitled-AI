//
//StockTrainingMethods.h
//
#ifndef STOCKTRAININGMETHODS
#define STOCKTRAININGMETHODS

#include "Network.h"
#include "Trainer.h"
#include <string>
#include <forward_list>

class StockTrainingMethods: public Trainer{

	public:
	StockTrainingMethods();
	int evaluate(Network&,Network&);
	double evaluate(Network&);
	int mutate(Network&,int);
	static std::string newDynamicTicTacToe(Network&);
	private:
	std::vector<double> openingVals;
	std::vector<double> closingVals;
	std::forward_list<int> convert(double,double);
	int randomLoc(std::forward_list<int>&);
	int choose(Network&);
};
#endif
