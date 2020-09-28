//
//TicTacToeTrainingMethods.cpp
//
#include "TicTacToeTrainingMethods.h"
#include "TicTacToe.h"

int TicTacToeTrainingMethods::choose(Network& n){
	if(n.recievers.empty())
		return 8;
	int numChoices = 11;
	int choices[] = {0,0,10,0,0,0,0,0,10,0,0};
	if(n.nextLocation() == -1){
		choices[0] = 0;
		choices[2] = 0;
		choices[3] = 0;
		choices[4] = 0;
		choices[5] = 0;
		choices[9] = 0;
		choices[10] = 0;
	}
	int randSize = 0;
	for(int i = 0;i < numChoices;i++)
		randSize += choices[i];
	int choice = std::rand() % randSize;
	for(int i = 0;i < numChoices;i++){
		choice -= choices[i];
		if(choice < 0)
			return i;
	}	
	return -1;
}

int TicTacToeTrainingMethods::mutate(Network& n,int choice = -1){
	for(int i = 0;i < NMUTATIONS;i++){
		if(choice == -1)
			choice = choose(n);
		switch(choice){
			case 2://add AND structure
			{
				int sender1 = n.randomReciever();
				std::forward_list<int> inputsLeft;
				std::forward_list<int> outputsLeft;
				for(int i = 1;i < 10;++i){
					inputsLeft.push_front(i);
					outputsLeft.push_front(i);
				}
				getInputsLeft(n,inputsLeft,sender1);
				getOutputsLeft(n,outputsLeft,sender1);
				int sender2 = randomLoc(inputsLeft);
				if(sender2 == -1){
					std::cout << sender1 << "alert\n";
					return -1;
				}
				inputsLeft.remove(sender2);
				outputsLeft.remove(sender2);
				int reciever = randomLoc(outputsLeft) + 9;
				int loc = n.nextLocation();
				std::forward_list<int> notTargets;
				for(auto recieversIterator = (*n.getNeuron(sender1)).getRecievers()->begin();recieversIterator != (*n.getNeuron(sender1)).getRecievers()->end();++recieversIterator){
					if((*n.getNeuron(*recieversIterator)).getPulse() > 0 && *recieversIterator > 18)
						notTargets.push_front(*recieversIterator);
				}
				int opponentWinCheck = n.nextLocation();
				n.addStructure(AND,sender1,sender2,reciever);
				for(auto targetsIterator = notTargets.begin();targetsIterator != notTargets.end();++targetsIterator){
					n.addStructure(NOT,loc,0,*targetsIterator);
					n.addStructure(NOT,*targetsIterator,0,loc);	
				}
				int inputsSize;
				std::vector<int> locInputs;
				if(inputsLeft.empty()){
					n.recievers.remove(sender1);
					locInputs.clear();
					n.getInputs(locInputs,sender1);
					inputsSize = 0;
					for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator)
						if((*n.getNeuron(*inputsIterator)).getPulse() > 0)
							inputsSize++;
					while(inputsSize > 1){
						for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator){
							if(*inputsIterator > 10 && (*n.getNeuron(*inputsIterator)).getPulse() > 0)
								sender1 = *inputsIterator;
						}						
						locInputs.clear();
						n.getInputs(locInputs,sender1);
						inputsSize = 0;
						for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator)
							if((*n.getNeuron(*inputsIterator)).getPulse() > 0)
								inputsSize++;
					}
					if(noRecievers(n,sender1,0)){
						recursiveAddReciever(n,sender1);
					}
				}
				int board[9];
				for(int i = 0;i < 9;++i)
					board[i] = 0;
				locInputs.clear();
				n.getInputs(locInputs,opponentWinCheck);
				inputsSize = 0;
				for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator)
					if((*n.getNeuron(*inputsIterator)).getPulse() > 0 && *inputsIterator > 9)
						inputsSize++;
				while(inputsSize > 0){
					for(auto recieversIterator = (*n.getNeuron(opponentWinCheck)).getRecievers()->begin();recieversIterator != (*n.getNeuron(opponentWinCheck)).getRecievers()->end();++recieversIterator){
						if(*recieversIterator < 19)
							board[*recieversIterator - 10] = 1;
					}
					for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator){
						if((*n.getNeuron(*inputsIterator)).getPulse() > 0){
							if(*inputsIterator > 9)
								opponentWinCheck = *inputsIterator;
							else
								if(*inputsIterator != 0){
									board[*inputsIterator - 1] = -1;
								}
						}
					}						
					locInputs.clear();
					n.getInputs(locInputs,opponentWinCheck);
					inputsSize = 0;
					for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator)
						if((*n.getNeuron(*inputsIterator)).getPulse() > 0 && *inputsIterator > 9)
							inputsSize++;
				}
				for(auto recieversIterator = (*n.getNeuron(opponentWinCheck)).getRecievers()->begin();recieversIterator != (*n.getNeuron(opponentWinCheck)).getRecievers()->end();++recieversIterator){
					if(*recieversIterator < 19)
						board[*recieversIterator - 10] = 1;
				}
				for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator){
					if((*n.getNeuron(*inputsIterator)).getPulse() > 0){
						if(*inputsIterator != 0){
							board[*inputsIterator - 1] = -1;
						}
					}
				}						
				
				TicTacToe t = TicTacToe();
				int* gBoard = t.getBoard();
				for(int i = 0;i < 9;i++)
					gBoard[i] = board[i];
				if(t.winner() == 1)
					return 1;
				if(t.oneAway(-1))
					return -1;
				if(t.twoAway(-1))
					return -1;
				if(t.threeAway(-1))
					return -1;
				return 1;
			}
			case 8://reciever
			{
				int loc = n.randomSender();
				if(std::rand() % 1 == 0 && !n.recievers.empty())
					loc = n.randomReciever();
				if(recursiveMutate(n,loc) == -1)
					return -1;
				clean(n);
				return 2;
			}
		}
	}
	return 0;
}

int TicTacToeTrainingMethods::noRecievers(Network& n,int loc,int depth){
	if(loc == 0)
		--depth;
	if(depth == 4)
		return 0;
	for(auto recieversIterator = (*n.getNeuron(loc)).getRecievers()->begin();recieversIterator != (*n.getNeuron(loc)).getRecievers()->end();++recieversIterator){
		for(auto checkIterator = n.recievers.begin();checkIterator != n.recievers.end();++checkIterator){
			if(*recieversIterator == *checkIterator)
				return 0;
		}
		if((*n.getNeuron(*recieversIterator)).getPulse() > 0 && noRecievers(n,*recieversIterator,depth + 1) == 0)
			return 0;
	}
	return 1;
}

void TicTacToeTrainingMethods::recursiveAddReciever(Network& n,int loc){
	int recieversSize = 0;
	for(auto recieverIterator = (*n.getNeuron(loc)).getRecievers()->begin();recieverIterator != (*n.getNeuron(loc)).getRecievers()->end();++recieverIterator)
		if((*n.getNeuron(*recieverIterator)).getPulse() > 0)
			++recieversSize;
	if(recieversSize > 1){
		for(auto recieverIterator = (*n.getNeuron(loc)).getRecievers()->begin();recieverIterator != (*n.getNeuron(loc)).getRecievers()->end();++recieverIterator)
			if((*n.getNeuron(*recieverIterator)).getPulse() > 0 && *recieverIterator > 18)
				recursiveAddReciever(n,*recieverIterator);
	}
	else{
		int board[9];
		for(int i = 1;i < 10;++i)
			board[i] = 0;
		int current = loc;
		std::vector<int> locInputs;
		n.getInputs(locInputs,loc);
		int inputsSize = 0;
		for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator)
			if((*n.getNeuron(*inputsIterator)).getPulse() > 0)
				inputsSize++;
		while(inputsSize > 1){
			for(auto recieversIterator = (*n.getNeuron(current)).getRecievers()->begin();recieversIterator != (*n.getNeuron(current)).getRecievers()->end();++recieversIterator){
				if(*recieversIterator < 19)
					board[*recieversIterator - 10] = 1;
			}
			for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator){
				if((*n.getNeuron(*inputsIterator)).getPulse() > 0){
					if(*inputsIterator > 10)
						current = *inputsIterator;
				}
			}						
			locInputs.clear();
			n.getInputs(locInputs,current);
			inputsSize = 0;
			for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator)
				if((*n.getNeuron(*inputsIterator)).getPulse() > 0)
					inputsSize++;
		}
		int win = 0;
		if(board[0] != 0 && board[0] == board[1] && board[0] == board[2])
			win = 1;
		if(board[3] != 0 && board[3] == board[4] && board[3] == board[5])
			win = 1;
		if(board[6] != 0 && board[6] == board[7] && board[6] == board[8])
			win = 1;
		if(board[0] != 0 && board[0] == board[3] && board[0] == board[6])
			win = 1;
		if(board[1] != 0 && board[1] == board[4] && board[1] == board[7])
			win = 1;
		if(board[2] != 0 && board[2] == board[5] && board[2] == board[8])
			win = 1;
		if(board[0] != 0 && board[0] == board[4] && board[0] == board[8])
			win = 1;
		if(board[6] != 0 && board[6] == board[4] && board[6] == board[2])
			win = 1;
		if(!win)
			n.recievers.push_front(loc);
	}
}

int TicTacToeTrainingMethods::recursiveMutate(Network& n,int loc){
	for(auto recieversIterator = (*n.getNeuron(loc)).getRecievers()->begin();recieversIterator != (*n.getNeuron(loc)).getRecievers()->end();++recieversIterator){
		if(*recieversIterator < 19){
			(*n.getNeuron(loc)).removeReciever(*recieversIterator);
			break;
		}
	}
	std::forward_list<int> outputsLeft;
	for(int i = 1;i < 10;++i){
		outputsLeft.push_front(i);
	}
	getOutputsLeft(n,outputsLeft,loc);
	while(!outputsLeft.empty()){
		int target = randomLoc(outputsLeft) + 9;
		outputsLeft.remove(target - 9);
		(*n.getNeuron(loc)).addReciever(target);
		if(loc > 29){
			std::forward_list<int> inputsLeft;
			for(int i = 1;i < 10;++i)
				inputsLeft.push_front(i);
			std::vector<int> locInputs;
			n.getInputs(locInputs,loc);
			int input = -1;
			int sender = -1;
			for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator){
				if(*inputsIterator > 18 && (*n.getNeuron(*inputsIterator)).getPulse() > 0)
					input = *inputsIterator;
				if(*inputsIterator < 10)
					sender = *inputsIterator;
			}
			getInputsLeft(n,inputsLeft,input);
			inputsLeft.remove(target - 9);
			inputsLeft.push_front(sender);
			while(!inputsLeft.empty()){
				(*n.getNeuron(sender)).removeReciever(loc);
				sender = randomLoc(inputsLeft);
				if(sender > 9)
					std::cout << "BAD\n";
				inputsLeft.remove(sender);
				(*n.getNeuron(sender)).addReciever(loc);
				int board[9];
				for(int i = 0;i < 9;++i)
					board[i] = 0;
				int opponentWinCheck = loc;
				std::vector<int> locInputs;
				n.getInputs(locInputs,opponentWinCheck);
				int inputsSize = 0;
				for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator)
					if((*n.getNeuron(*inputsIterator)).getPulse() > 0 && *inputsIterator > 9)
						inputsSize++;
				while(inputsSize > 0){
					for(auto recieversIterator = (*n.getNeuron(opponentWinCheck)).getRecievers()->begin();recieversIterator != (*n.getNeuron(opponentWinCheck)).getRecievers()->end();++recieversIterator){
						if(*recieversIterator < 19)
							board[*recieversIterator - 10] = 1;
					}
					for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator){
						if((*n.getNeuron(*inputsIterator)).getPulse() > 0){
							if(*inputsIterator > 9)
								opponentWinCheck = *inputsIterator;
							else
								if(*inputsIterator != 0){
									board[*inputsIterator - 1] = -1;
								}
						}
					}						
					locInputs.clear();
					n.getInputs(locInputs,opponentWinCheck);
					inputsSize = 0;
					for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator)
						if((*n.getNeuron(*inputsIterator)).getPulse() > 0 && *inputsIterator > 9)
							inputsSize++;
				}
				for(auto recieversIterator = (*n.getNeuron(opponentWinCheck)).getRecievers()->begin();recieversIterator != (*n.getNeuron(opponentWinCheck)).getRecievers()->end();++recieversIterator){
					if(*recieversIterator < 19)
						board[*recieversIterator - 10] = 1;
				}
				for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator){
					if((*n.getNeuron(*inputsIterator)).getPulse() > 0){
						if(*inputsIterator != 0){
							board[*inputsIterator - 1] = -1;
						}
						if(*inputsIterator > 9)
							std::cout << "very bad\n";
					}
				}						
				
				TicTacToe t = TicTacToe();
				int* gBoard = t.getBoard();
				for(int i = 0;i < 9;i++)
					gBoard[i] = board[i];
				gBoard[target - 10] = 0;
				if(t.oneAway(1) == 1){
					(*n.getNeuron(loc)).removeReciever(target);
					(*n.getNeuron(loc)).addReciever(t.getOneAway(1) + 10);
				}
				gBoard[target - 10] = 1;
				if(t.winner() == 1)
					return 1;
				if(!t.oneAway(-1) && !t.twoAway(-1)){
					int valid = 1;
					for(auto recieversIterator = (*n.getNeuron(loc)).getRecievers()->begin();recieversIterator != (*n.getNeuron(loc)).getRecievers()->end();++recieversIterator){
						if((*n.getNeuron(*recieversIterator)).getPulse() > 0 && *recieversIterator > 18 && valid)
							if(recursiveMutate(n,*recieversIterator) == -1)
								valid = 0;
					}
					if(valid)
						return 1;
				}
			}
		}
		else{
			int board[9];
			for(int i = 0;i < 9;++i)
				board[i] = 0;
			int opponentWinCheck = loc;
			std::vector<int> locInputs;
			n.getInputs(locInputs,opponentWinCheck);
			int inputsSize = 0;
			for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator)
				if((*n.getNeuron(*inputsIterator)).getPulse() > 0 && *inputsIterator > 9)
					inputsSize++;
			while(inputsSize > 0){
				for(auto recieversIterator = (*n.getNeuron(opponentWinCheck)).getRecievers()->begin();recieversIterator != (*n.getNeuron(opponentWinCheck)).getRecievers()->end();++recieversIterator){
					if(*recieversIterator < 19)
						board[*recieversIterator - 10] = 1;
				}
				for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator){
					if((*n.getNeuron(*inputsIterator)).getPulse() > 0){
						if(*inputsIterator > 9)
							opponentWinCheck = *inputsIterator;
						else
							if(*inputsIterator != 0){
								board[*inputsIterator - 1] = -1;
							}
					}
				}						
				locInputs.clear();
				n.getInputs(locInputs,opponentWinCheck);
				inputsSize = 0;
				for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator)
					if((*n.getNeuron(*inputsIterator)).getPulse() > 0 && *inputsIterator > 9)
						inputsSize++;
			}
			for(auto recieversIterator = (*n.getNeuron(opponentWinCheck)).getRecievers()->begin();recieversIterator != (*n.getNeuron(opponentWinCheck)).getRecievers()->end();++recieversIterator){
				if(*recieversIterator < 19)
					board[*recieversIterator - 10] = 1;
			}
			for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator){
				if((*n.getNeuron(*inputsIterator)).getPulse() > 0){
					if(*inputsIterator != 0){
						board[*inputsIterator - 1] = 1;
					}
				}
			}						
			
			TicTacToe t = TicTacToe();
			int* gBoard = t.getBoard();
			for(int i = 0;i < 9;i++)
				gBoard[i] = board[i];
			if(t.winner() == 1)
				return 1;
			if(!t.oneAway(-1) && !t.twoAway(-1) && !t.threeAway(-1)){
				int valid = 1;
				for(auto recieversIterator = (*n.getNeuron(loc)).getRecievers()->begin();recieversIterator != (*n.getNeuron(loc)).getRecievers()->end();++recieversIterator){
					if((*n.getNeuron(*recieversIterator)).getPulse() > 0 && *recieversIterator > 18 && valid)
						if(recursiveMutate(n,*recieversIterator) == -1)
							valid = 0;
				}
				if(valid)
					return 1;
			}

		}
		for(auto recieversIterator = (*n.getNeuron(loc)).getRecievers()->begin();recieversIterator != (*n.getNeuron(loc)).getRecievers()->end();++recieversIterator){
			if(*recieversIterator < 19){
				(*n.getNeuron(loc)).removeReciever(*recieversIterator);
				break;
			}
		}
	}
	return -1;
}

void TicTacToeTrainingMethods::getOutputsLeft(Network& n,std::forward_list<int>& outputsLeft,int loc){
	std::vector<int> locInputs;
	n.getInputs(locInputs,loc);
	int inputsSize = 0;
	for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator)
		if((*n.getNeuron(*inputsIterator)).getPulse() > 0)
			inputsSize++;
	int current = loc;
	while(inputsSize > 1){
		for(auto recieversIterator = (*n.getNeuron(current)).getRecievers()->begin();recieversIterator != (*n.getNeuron(current)).getRecievers()->end();++recieversIterator){
			if(*recieversIterator < 19){
				outputsLeft.remove(*recieversIterator - 9);
			}
		}
		for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator){
			if((*n.getNeuron(*inputsIterator)).getPulse() > 0){
				if(*inputsIterator < 10)
					outputsLeft.remove(*inputsIterator);
				else
					current = *inputsIterator;
			}
		}
		locInputs.clear();
		n.getInputs(locInputs,current);
		inputsSize = 0;
		for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator)
			if((*n.getNeuron(*inputsIterator)).getPulse() > 0)
				inputsSize++;
	}
	for(auto recieversIterator = (*n.getNeuron(current)).getRecievers()->begin();recieversIterator != (*n.getNeuron(current)).getRecievers()->end();++recieversIterator){
		outputsLeft.remove(*recieversIterator - 9);
	}
	for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator){
		if((*n.getNeuron(*inputsIterator)).getPulse() > 0){
			if(*inputsIterator < 10){
				outputsLeft.remove(*inputsIterator);
			}
		}
	}

}

void TicTacToeTrainingMethods::getInputsLeft(Network& n,std::forward_list<int>& inputsLeft,int loc){
	for(auto recieversIterator = (*n.getNeuron(loc)).getRecievers()->begin();recieversIterator != (*n.getNeuron(loc)).getRecievers()->end();++recieversIterator){
		if(*recieversIterator > 38 && (*n.getNeuron(*recieversIterator)).getPulse() > 0){
			std::vector<int> recieverInputs;
			n.getInputs(recieverInputs,*recieversIterator);
			for(auto inputsIterator = recieverInputs.begin();inputsIterator != recieverInputs.end();++inputsIterator){
				if(*inputsIterator < 10){
					inputsLeft.remove(*inputsIterator);
				}
			}
		}
	}
	std::vector<int> locInputs;
	n.getInputs(locInputs,loc);
	int inputsSize = 0;
	for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator)
		if((*n.getNeuron(*inputsIterator)).getPulse() > 0)
			inputsSize++;
	int current = loc;
	while(inputsSize > 1){
		for(auto recieversIterator = (*n.getNeuron(current)).getRecievers()->begin();recieversIterator != (*n.getNeuron(current)).getRecievers()->end();++recieversIterator){
			if(*recieversIterator < 19){
				inputsLeft.remove(*recieversIterator - 9);
			}
		}
		for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator){
			if((*n.getNeuron(*inputsIterator)).getPulse() > 0){
				if(*inputsIterator < 10){
					inputsLeft.remove(*inputsIterator);
				}
				else
					current = *inputsIterator;
			}
		}
		locInputs.clear();
		n.getInputs(locInputs,current);
		inputsSize = 0;
		for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator)
			if((*n.getNeuron(*inputsIterator)).getPulse() > 0)
				inputsSize++;
	}
	for(auto recieversIterator = (*n.getNeuron(current)).getRecievers()->begin();recieversIterator != (*n.getNeuron(current)).getRecievers()->end();++recieversIterator){
		inputsLeft.remove(*recieversIterator - 9);
	}
	for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator){
		if((*n.getNeuron(*inputsIterator)).getPulse() > 0){
			if(*inputsIterator < 10){
				inputsLeft.remove(*inputsIterator);
			}
		}
	}

}

std::string TicTacToeTrainingMethods::newDynamicTicTacToe(Network& n){
	n = Network();
	n.setDynamic(1);
	for(int i = 0;i < 29;i++){
                n.addNeuron(Neuron());
        }
        for(int i = 0;i < 10;i++)
                n.addInput(9 - i);
        for(int i = 0;i < 9;i++)
                n.addOutput(18 - i);
        for(int i = 0;i < 10;i++){
		(*n.getNeuron(i)).addReciever(i + 19);
		n.senders.push_front(i + 19);
		n.recievers.push_front(i + 19);
	}
        for(int i = 1;i < 10;i++){
		n.addStructure(NOT,0,0,i + 19);
	}
        for(int i = 1;i < 10;i++){
		for(int j = 1;j < 10;j++){
			if(i != j)
				n.addStructure(NOT,i,0,j + 19);
		}
	}
	return "Default Dynamic TicTacToe Network loaded";
}
int TicTacToeTrainingMethods::evaluate(Network &n){
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
		if(n.getDynamic())
			dynamicConvert(data,moveStack.top());
		else
			staticConvert(data,c.getBoard(),1);
		moveStack.pop();
		Network tn;
		tn = networkStack.top();
		networkStack.pop();
		int move = tn.process(&data) - 10;
		if(!n.getDynamic())
			move -= 9;
		if(!(c.move(move) < 0)){
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
					if(nextc.winner() == -1){
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
						score -= sum;
					}
                                }
			}
		}
//		else
//			if(move > 0)
//				std::cout << "bad move\n" << c.display() << " " << move << "\n" << n.info();
	}	

	return score;
}
int TicTacToeTrainingMethods::tevaluate(Network &n){
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
		if(n.getDynamic())
			dynamicConvert(data,moveStack.top());
		else
			staticConvert(data,c.getBoard(),1);
		moveStack.pop();
		Network tn;
		tn = networkStack.top();
		networkStack.pop();
		std::cout << c.display() << "\n";
		int move = tn.process(&data) - 10;
		if(!n.getDynamic())
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
void TicTacToeTrainingMethods::dynamicConvert(std::forward_list<int>&data, int loc){
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

void TicTacToeTrainingMethods::staticConvert(std::forward_list<int>&data,int *board,int turn){
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
int TicTacToeTrainingMethods::randomLoc(std::forward_list<int>& usedLocs){
	if(usedLocs.empty()){
		return -1;
	}
	else{
		int usedSize = 0;
		for(auto locsIterator = usedLocs.begin();locsIterator != usedLocs.end();++locsIterator)
			usedSize++;
		int choice = std::rand() % usedSize;
		auto usedIterator = usedLocs.begin();
		for(int i = 0;i < choice;i++)
			++usedIterator;
		return *usedIterator;
	}
}
void TicTacToeTrainingMethods::clean(Network& n){
	for(auto it = n.senders.begin();it != n.senders.end();++it){
		int opponentWinCheck = *it;
		int board[9];
		for(int i = 0;i < 9;++i)
			board[i] = 0;
		std::vector<int> locInputs;
		n.getInputs(locInputs,opponentWinCheck);
		int inputsSize = 0;
		for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator)
			if((*n.getNeuron(*inputsIterator)).getPulse() > 0 && *inputsIterator > 9)
				inputsSize++;
		while(inputsSize > 0){
			for(auto recieversIterator = (*n.getNeuron(opponentWinCheck)).getRecievers()->begin();recieversIterator != (*n.getNeuron(opponentWinCheck)).getRecievers()->end();++recieversIterator){
				if(*recieversIterator < 19)
					board[*recieversIterator - 10] = 1;
			}
			for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator){
				if((*n.getNeuron(*inputsIterator)).getPulse() > 0){
					if(*inputsIterator > 9)
						opponentWinCheck = *inputsIterator;
					else
						if(*inputsIterator != 0){
							board[*inputsIterator - 1] = -1;
						}
				}
			}						
			locInputs.clear();
			n.getInputs(locInputs,opponentWinCheck);
			inputsSize = 0;
			for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator)
				if((*n.getNeuron(*inputsIterator)).getPulse() > 0 && *inputsIterator > 9)
					inputsSize++;
		}
		for(auto recieversIterator = (*n.getNeuron(opponentWinCheck)).getRecievers()->begin();recieversIterator != (*n.getNeuron(opponentWinCheck)).getRecievers()->end();++recieversIterator){
			if(*recieversIterator < 19)
				board[*recieversIterator - 10] = 1;
		}
		for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator){
			if((*n.getNeuron(*inputsIterator)).getPulse() > 0){
				if(*inputsIterator != 0){
					board[*inputsIterator - 1] = -1;
				}
			}
		}						
		if(opponentWinCheck > 29 || opponentWinCheck < 19)
			std::cout << "owc\n";		
		TicTacToe t = TicTacToe();
		int* gBoard = t.getBoard();
		for(int i = 0;i < 9;i++)
			gBoard[i] = board[i];
		std::forward_list<int> inputsLeft;
		for(int i = 1;i < 10;++i)
			inputsLeft.push_front(i);
		getInputsLeft(n,inputsLeft,*it);
		if(t.winner() == 1 || inputsLeft.empty()){
			n.recievers.remove(*it);
			if(noRecievers(n,opponentWinCheck,0))
				recursiveAddReciever(n,opponentWinCheck);
		}
	}
}
void TicTacToeTrainingMethods::staticTraining(Network &n,std::vector<std::pair<std::forward_list<int>,int> > &targetData){
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
                                        trainPrecisionLearning(n,nextData);
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

int TicTacToeTrainingMethods::locate(std::vector<std::pair<std::forward_list<int>,int> > &targetData,std::forward_list<int> data){
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

int TicTacToeTrainingMethods::contains(std::vector<std::pair<std::forward_list<int>,int> > &targetData,std::forward_list<int> data){
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


void TicTacToeTrainingMethods::trainPrecisionLearning( Network &n, std::vector<std::pair<std::forward_list<int>,int> > &targetData) {
	
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
		
		int bitmap[n.getSize() / 16];//possibly change to increase speed
		int bitmapSize = n.getSize() * 2;
		for(int i = 0;i < bitmapSize / 32;i++)
			bitmap[i] = 0;
		for(auto inputsIterator = inputs.begin();inputsIterator != inputs.end();++inputsIterator){
			for(auto reciterator = (*n.getNeuron(*inputsIterator)).getRecievers()->begin(); reciterator != (*n.getNeuron(*inputsIterator)).getRecievers()->end();++reciterator){
				if((*n.getNeuron(*reciterator)).getPulse() > 0)
					bitmap[*reciterator / 32] |= 1<<((*reciterator) % 32);
			}
		}

		//create list of all the Neurons that contribute to the bitmap neurons

		std::forward_list<int> contributers;

		//used to determine if the given case has already been covered previously

		int covered = 0;

		if(inputs.front() == 0 && !(*n.getNeuron(0)).getRecievers()->empty())
			covered = 1;
		for(int i = 0;i < n.getSize();i++){
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
						if((*n.getNeuron(*oIterator)).getPulse() < 0)
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
                	        (*n.getNeuron(*inputsIterator)).addReciever(locs[0]);
                	}
			Neuron n1 = Neuron(inputsSize,2);
			n1.addReciever(locs[1]);
			n.addNeuron(n1);
			Neuron n2 = Neuron();
			n2.addReciever(locs[2]);
			n.addNeuron(n2);
			Neuron n3 = Neuron(1,2);
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
				for(int j = 0;j < n.getSize();j++){
					if(n.getIndex(j / 32) & 1<<(j % 32) && (*n.getNeuron(j)).getPulse() < 0){
						for(auto recieversIterator = (*n.getNeuron(j)).getRecievers()->begin();recieversIterator != (*n.getNeuron(j)).getRecievers()->end();++recieversIterator){
							if(*recieversIterator == (*n.getNeuron((*n.getNeuron(i)).getRecievers()->front())).getRecievers()->front())
								negator = j;//potential for bug if negator is 0
						}
					}
				}
				int reciever;
				
				if(negator){
					reciever = negator;
				}
				else{
					Neuron n4 = Neuron(1,-2);
                                	n4.addReciever((*n.getNeuron((*n.getNeuron(i)).getRecievers()->front())).getRecievers()->front());
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
                                                (*n.getNeuron(*contributersIterator)).removeReciever(reciever);
                                                (*n.getNeuron(*contributersIterator)).addReciever(reciever);
                                        }
                                }
				
			}
		}
	}
}
