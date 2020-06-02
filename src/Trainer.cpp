//
//Trainer.cpp
//

#include "Trainer.h"
#include <iterator>
#include <cstdlib>
#include <ctime>
#include <fstream>

void trainTargetLearning( Network &n, std::vector<std::pair<std::forward_list<int>,int> > &targetData, int enumerations) {
	std::srand(std::time(0));
	std::cout << "Training network for " << enumerations << " enumerations:\n";
	for(int i = 0;i < enumerations;i++){
		int iterations = 1000;
		Network copy;
		double accuracy, copyAccuracy;
		for(int j = 0;j < iterations;j++){
			std::cout << "\r";
			accuracy = 0;
			double total = 0;
			
			for(auto it = targetData.begin(); it != targetData.end();++it){
				if(n.process(&((*it).first)) == (*it).second){
					accuracy++;
				}
				total++;
			}
			accuracy /= total;
			std::cout << "|";
			int bars = 20;
			int numbars = iterations / bars;//right value is number of bars
			for(int k = 0;k <= j / numbars;k++)
				std::cout << "=";
			for(int k = 0;k < bars - (j / numbars) - 1;k++)
				std::cout << " ";
			std::cout << "|  ";
			std::cout << (accuracy * 100) << "% accuracy";
			std::cout << std::flush;
			if(accuracy == 1){
				std::cout << "\n" << n;
				for(auto it = targetData.begin(); it != targetData.end();++it){
					std::cout << "\n" << n.process(&((*it).first));
				}
				return;
			}


			copy = n;
			copy.mutate();
			copyAccuracy = 0; 
			for(auto it = targetData.begin(); it != targetData.end();++it){
                                if(copy.process(&((*it).first)) == (*it).second)
                                        copyAccuracy++;
                        }
                        copyAccuracy /= total;
			if(copyAccuracy > accuracy){
				n = copy;
			}			
		}
		std::cout << "\n\n";
	}
	std::ofstream file;
	file.open("../test.txt");
	file << n;
	file.close();
}
