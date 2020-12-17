//
//main.cpp
//

#include "src/Trainer.h"
#include "src/StockTrainingMethods.h"
#include "src/Network.h"
#include <utility>
#include <forward_list>
#include <cstdlib>
#include <thread>
#include <iostream>
#include <fstream>



int main(){
	std::srand(std::time(0));

	int end = 0;
	std::ofstream outFile("data.csv");

	StockTrainingMethods StockTrainer = StockTrainingMethods();

	for(int i = 0;i < 100;++i){
		Network n;
		StockTrainingMethods::newDynamicTicTacToe(n);
		StockTrainer.dynamicTraining(&end, std::ref(n), 10 + (i / 10 ) * 10);
		for(int j = 0;j < 10;++j){
			double val = StockTrainer.evaluate(n);
			outFile << (10 + (i / 10) * 10) << ", " << val << "\n";
		}
	}
	outFile.close();
	exit(0);
}
