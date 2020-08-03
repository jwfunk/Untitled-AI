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
	
	for(auto targetIterator = targetData.begin();targetIterator != targetData.end();++targetIterator){

		//Put the inputs values into inputs

		int inputsSize = 0;
		int inputLoc = 0;
		std::forward_list<int> inputs;
		for(auto firstIterator = (*targetIterator).first.begin();firstIterator != (*targetIterator).first.end();++firstIterator){
			if(*firstIterator == 1){
				inputs.push_front(inputLoc);
				inputsSize++;
			}
			inputLoc++;
		}

		

		std::vector<int> locs = n.nextLocations(3);
                if(locs.empty())
                	return;
		
		//create a bitmap to keep track of what neurons the active inputs are affecting
		
		int bitmap[n.size / 16];//possibly change to increase speed
		int bitmapSize = n.size * 2;
		for(int i = 0;i < bitmapSize / 32;i++)
			bitmap[i] = 0;
		for(auto inputsIterator = inputs.begin();inputsIterator != inputs.end();++inputsIterator){
			for(auto reciterator = n.neurons[*inputsIterator].recievers.begin();reciterator != n.neurons[*inputsIterator].recievers.end();++reciterator){
				if(n.neurons[*reciterator].pulse > 0)
					bitmap[*reciterator / 32] |= 1<<((*reciterator) % 32);
			}
		}

		//create list of all the Neurons that contribute to the bitmap neurons

		std::forward_list<int> contributers;

		//used to determine if the given case has already been covered previously

		int covered = 0;

		if(inputs.front() == 0 && !n.neurons[0].recievers.empty())
			covered = 1;
		for(int i = 0;i < n.size;i++){
			if(bitmap[i / 32] & 1<<(i % 32)){
				std::vector<int> oTree;
				n.outputTree(oTree,i);
				int inputsCheck[inputsSize + 1];
                                for(int j = 0;j < inputsSize + 1;j++){
                                        inputsCheck[j] = 0;
                                }
				for(auto oIterator = oTree.begin();oIterator != oTree.end();++oIterator){
					//Identify if case handled
					int outside = 1;
					int j = 1;				
					for(auto inputsIterator = inputs.begin();inputsIterator != inputs.end();++inputsIterator){
						if(n.neurons[*oIterator].pulse < 0)
							outside = 0;
						if(*inputsIterator == *oIterator){
							outside = 0;
							inputsCheck[j] = 1;	
						}
						j++;
					}
					if(i == *oIterator)
						outside = 0;
					if(outside)
						inputsCheck[0] = 1;
					//Identify if case handled				
	
					int contains = 0;
					for(auto contributersIterator = contributers.begin();contributersIterator != contributers.end();++contributersIterator){
						if(*contributersIterator == *oIterator)
							contains = 1;
					}
					if(!contains && i != *oIterator){
						contributers.push_front(*oIterator);
					}
				}

				//Identify if case handled
				int all = 1;
				for(int i = 1;i < inputsSize + 1;i++){
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

			//create neuron tree to cover the new case
			std::vector<int> locs = n.nextLocations(3);
                	if(locs.empty())
                		return;

			for(auto inputsIterator = inputs.begin();inputsIterator != inputs.end();++inputsIterator){
                	        n.neurons[*inputsIterator].addReciever(locs[0]);
                	}
			Neuron n1 = Neuron();
			n1.addReciever(locs[1]);
			n1.criticalCharge = inputsSize;
			n1.pulse = 2;
			n.addNeuron(n1);
			Neuron n2 = Neuron();
			n2.addReciever(locs[2]);
			n.addNeuron(n2);
			Neuron n3 = Neuron();
			n3.criticalCharge = 2;
			n3.addReciever((*targetIterator).second);
			n.addNeuron(n3);
			bitmap[locs[0] / 32] |= 1<<(locs[0] % 32);
		}

		//make sure that all the bitmap neurons are properly negated

		for(int i = 0;i < bitmapSize;i++){
			if(bitmap[i / 32] & 1<<(i % 32)){
				std::vector<int> oTree;
                                n.outputTree(oTree,i);
				int negator = 0;
				for(int j = 0;j < n.size;j++){
					if(n.index[j / 32] & 1<<(j % 32) && n.neurons[j].pulse < 0){
						for(auto recieversIterator = n.neurons[j].recievers.begin();recieversIterator != n.neurons[j].recievers.end();++recieversIterator){
							if(*recieversIterator == n.neurons[n.neurons[i].recievers.front()].recievers.front())
								negator = j;//potential for bug if negator is 0
						}
					}
				}
				int reciever;
				
				if(negator){
					reciever = negator;
				}
				else{
					Neuron n4 = Neuron();
                                	n4.addReciever(n.neurons[n.neurons[i].recievers.front()].recievers.front());
                                	n4.pulse = -2;
                                	int loc4 = n.nextLocation();
                                	if(loc4 == -1)
                                	        return;
					reciever = loc4;
                                	n.addNeuron(n4);	
				}
				for(auto contributersIterator = contributers.begin();contributersIterator != contributers.end();++contributersIterator){
                                        int present = 0;
                                        for(auto oIterator = oTree.begin();oIterator != oTree.end();++oIterator){
                                                if(*oIterator == *contributersIterator)
                                                        present = 1;
                                        }
                                        if(!present){
                                                n.neurons[*contributersIterator].removeReciever(reciever);
                                                n.neurons[*contributersIterator].addReciever(reciever);
                                        }
                                }
				
			}
		}
	}
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
