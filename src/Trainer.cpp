//
//Trainer.cpp
//

#include "Trainer.h"
#include <iterator>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <forward_list>

void Trainer::trainPrecisionLearning( Network &n, std::vector<std::pair<std::forward_list<int>,int> > &targetData) {
	std::srand(std::time(0));
	int size;
	std::forward_list<int> inputs;
	for(auto it = targetData.begin();it != targetData.end();++it){
		size = 0;
		int i = 0;
		for(auto itt = (*it).first.begin();itt != (*it).first.end();++itt){
			if(*itt == 1){
				inputs.push_front(i);
				size++;
			}
			i++;
		}
		std::vector<int> locs = n.nextLocations(3);
                if(locs.empty())
                	return;
		int bitmap[n.size / 32];
		for(int i = 0;i < n.size / 32;i++)
			bitmap[i] = 0;
		for(auto itt = inputs.begin();itt != inputs.end();++itt){
			for(auto reciterator = n.neurons[*itt].recievers.begin();reciterator != n.neurons[*itt].recievers.end();++reciterator){
				bitmap[*reciterator / 32] |= 1<<((*reciterator) % 32);
			}
		}

		bitmap[locs[0] / 32] &= ~(1 << (locs[0] % 32));
		std::forward_list<int> contributers;

		int covered = 0;

		if(inputs.front() == 0 && !n.neurons[0].recievers.empty())
			covered = 1;
		for(int i = 0;i < n.size;i++){
			if(bitmap[i / 32] & 1<<(i % 32)){
				std::vector<int> oTree;
				n.outputTree(oTree,i);
				int inputsCheck[size + 1];
                                for(int k = 0;k < size + 1;k++){
                                        inputsCheck[k] = 0;
                                }
				for(auto itt = oTree.begin();itt != oTree.end();++itt){
					//Identify if case handled
					int outside = 1;
					int j = 1;				
					for(auto ittt = inputs.begin();ittt != inputs.end();++ittt){
						if(n.neurons[*itt].pulse < 0)
							outside = 0;
						if(*ittt == *itt){
							outside = 0;
							inputsCheck[j] = 1;	
						}
						j++;
					}
					if(i == *itt)
						outside = 0;
					if(outside)
						inputsCheck[0] = 1;
					//Identify if case handled				
	
					int contains = 0;
					for(auto inputit = inputs.begin();inputit != inputs.end();++inputit){
						if(*inputit == *itt)
							contains = 1;
					}
					for(auto contriit = contributers.begin();contriit != contributers.end();++contriit){
						if(*contriit == *itt)
							contains = 1;
					}
					if(!contains && i != *itt){
						contributers.push_front(*itt);
					}
				}

				//Identify if case handled
				int all = 1;
				for(int i = 1;i < size + 1;i++){
					if(inputsCheck[i] == 0)
						all = 0;
				}
				if(inputsCheck[0] == 1)
					all = 0;
				if(all)
					covered = 1;
				//Identify if case handled
			}
		}

		
		if(!covered){
			for(auto itt = inputs.begin();itt != inputs.end();++itt){
                	        n.neurons[*itt].addReciever(locs[0]);
                	}
			Neuron n1 = Neuron();
			n1.addReciever(locs[1]);
			n1.criticalCharge = size;
			n1.pulse = 2;
			n.addNeuron(n1);
			Neuron n2 = Neuron();
			n2.addReciever(locs[2]);
			n.addNeuron(n2);
			Neuron n3 = Neuron();
			n3.criticalCharge = 2;
			n3.addReciever((*it).second);
			n.addNeuron(n3);
		}
		std::cout << "\n";
		if(!contributers.empty()){
			int negator = 0;
			for(int i = 0;i < n.size;i++){
				if(n.index[i / 32] & 1<<(i % 32)){
					for(auto itt = n.neurons[i].recievers.begin();itt != n.neurons[i].recievers.end();++itt){
						if(*itt == locs[2] && n.neurons[i].pulse < 0)
							negator = i;
					}
				}
			}
			if(negator){
				for(auto itt = contributers.begin();itt != contributers.end();++itt){
					n.neurons[*itt].removeReciever(negator);
					n.neurons[*itt].addReciever(negator);
				}
			}
			else{
				Neuron n4 = Neuron();
				n4.addReciever(locs[2]);
				n4.pulse = -2;
				int loc4 = n.nextLocation();
				if(loc4 == -1)
					return;
				n.addNeuron(n4);
				for(auto itt = contributers.begin();itt != contributers.end();++itt){
					n.neurons[*itt].addReciever(loc4);
				}
			}
		}
	}
	double accuracy = 0;
        double total = 0;
	for(auto it = targetData.begin(); it != targetData.end();++it){

                if(n.process(&((*it).first)) == (*it).second){
                        accuracy++;
                }
                total++;
        }
        accuracy /= total;
	std::cout << "\n\n" << (accuracy * 100) << "% accuracy\n";
}

void Trainer::trainTargetLearning( Network &n, std::vector<std::pair<std::forward_list<int>,int> > &targetData, int enumerations) {
	std::srand(std::time(0));
	std::cout << "Training network for " << enumerations << " enumerations:\n";
	std::forward_list<int> accurate;	
	double accuracy = 0;
        double total = 0;
	int size = 0;
        for(auto it = targetData.begin(); it != targetData.end();++it){
                
                if(n.process(&((*it).first)) == (*it).second){
                	accuracy++;
			accurate.push_front(size);
                }
        	total++;
		size++;
        }
	accuracy /= total;
	for(int i = 0;i < enumerations;i++){
		int iterations = 100000;
		Network copy;
		double copyAccuracy;
		std::forward_list<int> copyAccurate;
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


			copy = n;

			for(int i = 0;i < NMUTATIONS;i++)
				copy.mutate(-1);
			int c = 0;
			for(auto it = accurate.begin();it != accurate.end();++it){
				if(*it != c){
					copy.mutateTarget(targetData[c]);
					break;
				}
				c++;
			}
			//copy.mutateTarget(targetData[std::rand() % size]);

			copyAccuracy = 0; 
			copyAccurate.clear();
			c = 0;
			for(auto it = targetData.begin(); it != targetData.end();++it){

                                if(copy.process(&((*it).first)) == (*it).second){
                                        copyAccuracy++;
					copyAccurate.push_front(c);
				}
				c++;
                        }
                        copyAccuracy /= total;
			if(copyAccuracy > accuracy){

				n = copy;
				accuracy = copyAccuracy;
				accurate = copyAccurate;
			}			
		} 
		std::cout << "\n\n";
	}




	
	std::ofstream file;
	file.open("../test.txt");
	file << n;
	file.close();
}
