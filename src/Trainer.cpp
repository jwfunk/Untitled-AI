//
//Trainer.cpp
//

#include "Trainer.h"
#include <iterator>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include "Timer.h"
Timer timerProcess;
Timer timerMutate;
Timer timerTotal;
Timer timerCopy;

void trainTargetLearning( Network &n, std::vector<std::pair<std::forward_list<int>,int> > &targetData, int enumerations) {
	std::srand(std::time(0));
	std::cout << "Training network for " << enumerations << " enumerations:\n";
	timerTotal.Reset();
	timerMutate.Reset();
	timerProcess.Reset();
	timerCopy.Reset();
	timerTotal.Start();
	double accuracy = 0;
        double total = 0;
        for(auto it = targetData.begin(); it != targetData.end();++it){
                timerProcess.Restart();
                if(n.process(&((*it).first)) == (*it).second){
                	accuracy++;
                }
                timerProcess.Pause();
        	total++;
        }
	accuracy /= total;
	for(int i = 0;i < enumerations;i++){
		int iterations = 10000;
		Network copy;
		double copyAccuracy;
		
		for(int j = 0;j < iterations;j++){
			std::cout << "\r";
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

			timerCopy.Restart();
			copy = n;
			timerCopy.Pause();
			timerMutate.Restart();
			copy.mutate();
			timerMutate.Pause();
			copyAccuracy = 0; 
			for(auto it = targetData.begin(); it != targetData.end();++it){
				timerProcess.Restart();
                                if(copy.process(&((*it).first)) == (*it).second)
                                        copyAccuracy++;
				timerProcess.Pause();
                        }
                        copyAccuracy /= total;
			if(copyAccuracy > accuracy){
				timerCopy.Restart();
				n = copy;
				accuracy = copyAccuracy;
				timerCopy.Pause();
			}			
		} 
		std::cout << "\n\n";
	}
	timerTotal.Pause();
	timerTotal.Print("Total time :");
	timerProcess.Print("Process time :");
	timerMutate.Print("Mutate time :");
	timerCopy.Print("Copy time :");
	std::ofstream file;
	file.open("../test.txt");
	file << n;
	file.close();
}
