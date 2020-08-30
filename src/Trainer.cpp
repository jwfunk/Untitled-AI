//
//Trainer.cpp
//

#include "Trainer.h"
#include <iterator>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <forward_list>
#include "TicTacToe.h"
#include <stack>
#include <tuple>

void Trainer::dynamicTraining(Network &n, int enumerations){
	n.setDynamic(1);
	int nMutations = 0;
	int nVal = evaluate(n);
	for(int i = 0;i < enumerations;i++){
		Network c;//need copy constructor
		c = n;
		c.mutate(-1);
		int nWins = 0;
		int cWins = 0;
		for(int i = 0;i < 20;++i){
			int winner = compete(n,c);
			n.clear();
			c.clear();
			if(winner == 1)
				nWins++;
			if(winner == -1)
				cWins++;
		}
		if(cWins >= nWins){
			int cVal = evaluate(c);
			if(cVal > nVal){
				n = c;
				nVal = cVal;
				nMutations++;
				std::cout << cVal << " \n";
			}
		}
		std::cout << "|";
                int bars = 100;
                int numbars = enumerations / bars;//right value is number of bars
                for(int k = 0;k <= i / numbars;k++)
                        std::cout << "=";
                for(int k = 0;k < bars - (i / numbars) - 1;k++)
                        std::cout << " ";
                std::cout << "|  ";
		std::cout << i << " " << nMutations << "\r" << std::flush;
	}
}

int Trainer::compete(Network& n,Network& c){
	int winner = 0;
	std::forward_list<int> data;
	int pmoven = -1;
	int pmovec = -1;
	TicTacToe tn, tc;
	if(std::rand() % 2){
		pmoven = std::rand() % 9;
		pmovec = pmoven;
		tn.swap();
		tc.swap();
		tn.move(pmoven);
		tc.move(pmovec);
	}
	while(winner == 0){
		dynamicConvert(data,pmoven);
		int rValn = tn.move(n.process(&data) - 10);
		dynamicConvert(data,pmovec);
		int rValc = tc.move(n.process(&data) - 10);
		if(rValc == -1 && rValn == -1)
			return 0;
		if(rValc == -1)
			return 1;
		if(rValn == -1)
			return -1;
		if(rValc == 1 && rValn == 1){
			if(tn.winner() == 1 && tc.winner() == 1)
				return 0;
			if(tn.winner() == 1)
				return 1;
			if(tc.winner() == 1)
				return -1;
			return 0;
		}
		if(rValn == 1)
			return 1;
		if(rValc == 1)
			return -1;
		do{
			pmoven = std::rand() % 9;
		}
		while((rValn = tn.move(pmoven)) == -1);
		pmovec = pmoven;
		while((rValc = tc.move(pmovec)) == -1){
			pmovec = std::rand() % 9;
		}
		if(rValc == 1 && rValn == 1){
			if(tn.winner() == -1 && tc.winner() == -1)
				return 0;
			if(tn.winner() == -1)
				return -1;
			if(tc.winner() == -1)
				return 1;
			return 0;	
		}
		if(rValn == 1)
			return -1;
		if(rValc == 1)
			return 1;
	}
}

void Trainer::staticTraining(Network &n,std::vector<std::pair<std::forward_list<int>,int> > &targetData){
	system("clear");
	std::vector<std::pair<std::forward_list<int>,int> > nextData;
        TicTacToe t = TicTacToe();
	t.swap();
	std::stack<TicTacToe> aiMoves;
	for(int i = 0;i < 9;i++){
		TicTacToe c = t;
		c.move(i);
		aiMoves.push(c);
	}
	t.swap();
	aiMoves.push(t);
	while(!aiMoves.empty()){
		double accuracy = 0;
                double total = 0;

                //debugging purposes
                for(auto targetIterator = targetData.begin(); targetIterator != targetData.end();++targetIterator){

                        if(n.process(&((*targetIterator).first)) == (*targetIterator).second){
                                accuracy++;
                        }
                        else{
                                for(auto firstIterator = (*targetIterator).first.begin();firstIterator != (*targetIterator).first.end();++firstIterator)
                                        std::cout << (*firstIterator) << " ";
                        }
                        total++;
                }
                accuracy /= total;
                std::cout << "\n\n" << (accuracy * 100) << "% accuracy\n";
                //debugging purposes
		t = aiMoves.top();
		aiMoves.pop();
		std::cout << t.display();
                std::forward_list<int> data;
                staticConvert(data,t.getBoard(),t.getTurn());
                int move = -20;
                        if(contains(targetData,data)){
                                move = locate(targetData,data) - 19;
                                t.move(move);
                        }       
                        else{   
                                std::cout << "make your move\n";
                                std::cin >> move;
                                if(move == -1)
                                        return;
                                int won = 0;
				while((won = t.move(move)) == -1)
                                        std::cin >> move;
                                
                                //add new case to targetData
                                std::pair<std::forward_list<int>,int> p;
                                p.first = data;
                                p.second = move + 19;
                                targetData.push_back(p);

                                nextData.push_back(p);
                                if(n.process(&data) - 19 != move)
                                        Trainer::trainPrecisionLearning(n,nextData);
                                nextData.clear();
				if(won != 1){
					for(int i = 0;i < 9;i++){
                				TicTacToe c = t;
                				if((won = c.move(i)) != -1 && won != 1)
                					aiMoves.push(c);
        				}
				}
                        }
                system("clear");
	}
	//return value
}

int Trainer::locate(std::vector<std::pair<std::forward_list<int>,int> > &targetData,std::forward_list<int> data){
        for(auto targetIterator = targetData.begin();targetIterator != targetData.end();++targetIterator){
                auto dataIterator = data.begin();
                int match = 1;
                for(auto firstIterator = (*targetIterator).first.begin();firstIterator != (*targetIterator).first.end();++firstIterator){
                        if(*firstIterator != *dataIterator)
                                match = 0;
                        ++dataIterator;
                }
                if(match)
                        return (*targetIterator).second;
        }
        return -1;
}

int Trainer::contains(std::vector<std::pair<std::forward_list<int>,int> > &targetData,std::forward_list<int> data){
        for(auto targetIterator = targetData.begin();targetIterator != targetData.end();++targetIterator){
                auto dataIterator = data.begin();
                int match = 1;
                for(auto firstIterator = (*targetIterator).first.begin();firstIterator != (*targetIterator).first.end();++firstIterator){
                        if(*firstIterator != *dataIterator)
                                match = 0;
                        ++dataIterator;
                }
                if(match)
                        return 1;
        }
        return 0;
}

void Trainer::dynamicConvert(std::forward_list<int>&data, int loc){
	data.clear();
	int noInputs = 1;
	for(int i = 8;i >= 0;i--){
		if(i == loc){
                        data.push_front(1);
                        noInputs = 0;
                }
                else{
                        data.push_front(0);
                }
	}
        if(noInputs)
                data.push_front(1);
        else
                data.push_front(0);
}

void Trainer::staticConvert(std::forward_list<int>&data,int *board,int turn){
	data.clear();
        int noInputs = 1;
        int oTurn;
        if(turn == 1)
                oTurn = -1;
        else
                oTurn = 1;
        for(int i = 8;i >= 0;i--){
                if(board[i] == oTurn){
                        data.push_front(1);
                        noInputs = 0;
                }
                else{
                        data.push_front(0);
                }
        }
        for(int i = 8;i >= 0;i--){
                if(board[i] == turn){
                        data.push_front(1);
                        noInputs = 0;
                }
                else{
                        data.push_front(0);
                }
        }
        if(noInputs)
                data.push_front(1);
        else
                data.push_front(0);
}

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
				for(int j = 1;j < inputsSize + 1;j++){
					if(inputsCheck[j] == 0)
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

int Trainer::evaluate(Network &n){
	TicTacToe t = TicTacToe();
        t.swap();
        std::stack<TicTacToe> boardStack;
	std::stack<int> moveStack;
	std::stack<Network> networkStack;
        for(int i = 0;i < 9;i++){
                TicTacToe c = t;
                c.move(i);
		boardStack.push(c);
		moveStack.push(i);
		Network cn;
		cn = n;
		networkStack.push(cn);	
        }
        t.swap();
	boardStack.push(t);
	moveStack.push(-1);
	Network cn;
	cn = n;
	networkStack.push(cn);	
	int score = 0;

	while(!boardStack.empty()){
		std::forward_list<int> data;
		TicTacToe c = boardStack.top();
		boardStack.pop();
		if(n.dynamic)
			dynamicConvert(data,moveStack.top());
		else
			staticConvert(data,c.getBoard(),1);
		moveStack.pop();
		Network tn;
		tn = networkStack.top();
		networkStack.pop();
		int move = tn.process(&data) - 10;
		if(!n.dynamic)
			move -= 9;
		if(c.move(move) != -1){
			score++;
			if(c.winner() == 1){
				int* board = c.getBoard();
				int win = 0;
				int sum = 1;
				for(int i = 0;i < 9;i++){
					if(board[i] == 0){
						win++;
						sum *= win;
					}
				}
				sum++;
				score += sum;
			}
			else{
				for(int i = 0;i < 9;i++){
					TicTacToe nextc = c;
					int won = nextc.move(i);
					if(won != -1 && won != 1){
						boardStack.push(nextc);
						moveStack.push(i);
						networkStack.push(tn);
					}
					if(nextc.winner() == -1)
						score--;
                                }
			}
		}
	}

	return score;
}
int Trainer::tevaluate(Network &n){
	TicTacToe t = TicTacToe();
        t.swap();
        std::stack<TicTacToe> boardStack;
	std::stack<int> moveStack;
	std::stack<Network> networkStack;
        for(int i = 0;i < 9;i++){
                TicTacToe c = t;
                c.move(i);
		boardStack.push(c);
		moveStack.push(i);
		Network cn;
		cn = n;
		networkStack.push(cn);	
        }
        t.swap();
	boardStack.push(t);
	moveStack.push(-1);
	Network cn;
	cn = n;
	networkStack.push(cn);	
	int score = 0;

	while(!boardStack.empty()){
		std::forward_list<int> data;
		TicTacToe c = boardStack.top();
		boardStack.pop();
		if(n.dynamic)
			dynamicConvert(data,moveStack.top());
		else
			staticConvert(data,c.getBoard(),1);
		moveStack.pop();
		Network tn;
		tn = networkStack.top();
		networkStack.pop();
		std::cout << c.display() << "\n";
		int move = tn.process(&data) - 10;
		if(!n.dynamic)
			move -= 9;
		std::cout << move << "\n";
		if(c.move(move) != -1){
			score++;
			if(c.winner() == 1){
				int* board = c.getBoard();
				int win = 0;
				int sum = 1;
				for(int i = 0;i < 9;i++){
					if(board[i] == 0){
						win++;
						sum *= win;
					}
				}
				sum++;
				score += sum;
			}
			else{
				for(int i = 0;i < 9;i++){
					TicTacToe nextc = c;
					int won = nextc.move(i);
					if(won != -1 && won != 1){
						boardStack.push(nextc);
						moveStack.push(i);
						networkStack.push(tn);
					}
					if(nextc.winner() == -1)
						score--;
                                }
			}
		}
	}

	return score;
}
