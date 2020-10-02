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
				Neuron recieversNeuron = n.getNeuron(sender1);
				for(auto recieversIterator = recieversNeuron.recieversBegin();recieversIterator != recieversNeuron.recieversEnd();++recieversIterator){
					if(n.getNeuron(*recieversIterator).getPulse() > 0 && *recieversIterator > 18)
						notTargets.push_front(*recieversIterator);
				}
				int opponentWinCheck = n.nextLocation();
				n.addStructure(Network::AND,sender1,sender2,reciever);
				for(auto targetsIterator = notTargets.begin();targetsIterator != notTargets.end();++targetsIterator){
					n.addStructure(Network::NOT,loc,0,*targetsIterator);
					n.addStructure(Network::NOT,*targetsIterator,0,loc);	
				}
				int inputsSize;
				std::vector<int> locInputs;
				if(inputsLeft.empty()){
					n.recievers.remove(sender1);
					locInputs = n.getInputs(sender1);
					inputsSize = 0;
					for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator)
						if(n.getNeuron(*inputsIterator).getPulse() > 0)
							inputsSize++;
					while(inputsSize > 1){
						for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator){
							if(*inputsIterator > 10 && n.getNeuron(*inputsIterator).getPulse() > 0)
								sender1 = *inputsIterator;
						}						
						locInputs = n.getInputs(sender1);
						inputsSize = 0;
						for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator)
							if(n.getNeuron(*inputsIterator).getPulse() > 0)
								inputsSize++;
					}
					if(noRecievers(n,sender1,0)){
						recursiveAddReciever(n,sender1);
					}
				}
				int board[9];
				for(int i = 0;i < 9;++i)
					board[i] = 0;
				locInputs = n.getInputs(opponentWinCheck);
				inputsSize = 0;
				for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator)
					if(n.getNeuron(*inputsIterator).getPulse() > 0 && *inputsIterator > 9)
						inputsSize++;
				while(inputsSize > 0){
					Neuron recieversNeuron = n.getNeuron(opponentWinCheck);
					for(auto recieversIterator = recieversNeuron.recieversBegin(); recieversIterator != recieversNeuron.recieversEnd(); ++recieversIterator){
						if(*recieversIterator < 19)
							board[*recieversIterator - 10] = 1;
					}
					for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator){
						if(n.getNeuron(*inputsIterator).getPulse() > 0){
							if(*inputsIterator > 9)
								opponentWinCheck = *inputsIterator;
							else
								if(*inputsIterator != 0){
									board[*inputsIterator - 1] = -1;
								}
						}
					}						
					locInputs = n.getInputs(opponentWinCheck);
					inputsSize = 0;
					for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator)
						if(n.getNeuron(*inputsIterator).getPulse() > 0 && *inputsIterator > 9)
							inputsSize++;
				}
				recieversNeuron = n.getNeuron(opponentWinCheck);
				for(auto recieversIterator = recieversNeuron.recieversBegin();recieversIterator != recieversNeuron.recieversEnd();++recieversIterator){
					if(*recieversIterator < 19)
						board[*recieversIterator - 10] = 1;
				}
				for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator){
					if(n.getNeuron(*inputsIterator).getPulse() > 0){
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
	Neuron recieversNeuron = n.getNeuron(loc);
	for(auto recieversIterator = recieversNeuron.recieversBegin();recieversIterator != recieversNeuron.recieversEnd();++recieversIterator){
		for(auto checkIterator = n.recievers.begin();checkIterator != n.recievers.end();++checkIterator){
			if(*recieversIterator == *checkIterator)
				return 0;
		}
		if(n.getNeuron(*recieversIterator).getPulse() > 0 && noRecievers(n,*recieversIterator,depth + 1) == 0)
			return 0;
	}
	return 1;
}

void TicTacToeTrainingMethods::recursiveAddReciever(Network& n,int loc){
	int recieversSize = 0;
	Neuron recieversNeuron = n.getNeuron(loc);
	for(auto recieverIterator = recieversNeuron.recieversBegin();recieverIterator != recieversNeuron.recieversEnd();++recieverIterator)
		if(n.getNeuron(*recieverIterator).getPulse() > 0)
			++recieversSize;
	if(recieversSize > 1){
		for(auto recieverIterator = recieversNeuron.recieversBegin();recieverIterator != recieversNeuron.recieversEnd();++recieverIterator)
			if(n.getNeuron(*recieverIterator).getPulse() > 0 && *recieverIterator > 18)
				recursiveAddReciever(n,*recieverIterator);
	}
	else{
		int board[9];
		for(int i = 1;i < 10;++i)
			board[i] = 0;
		int current = loc;
		std::vector<int> locInputs = n.getInputs(loc);
		int inputsSize = 0;
		for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator)
			if(n.getNeuron(*inputsIterator).getPulse() > 0)
				inputsSize++;
		while(inputsSize > 1){
			Neuron recieversNeuron = n.getNeuron(current);
			for(auto recieversIterator = recieversNeuron.recieversBegin();recieversIterator != recieversNeuron.recieversEnd();++recieversIterator){
				if(*recieversIterator < 19)
					board[*recieversIterator - 10] = 1;
			}
			for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator){
				if(n.getNeuron(*inputsIterator).getPulse() > 0){
					if(*inputsIterator > 10)
						current = *inputsIterator;
				}
			}						
			locInputs = n.getInputs(current);
			inputsSize = 0;
			for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator)
				if(n.getNeuron(*inputsIterator).getPulse() > 0)
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
	Neuron recieversNeuron = n.getNeuron(loc);
	for(auto recieversIterator = recieversNeuron.recieversBegin();recieversIterator != recieversNeuron.recieversEnd();++recieversIterator){
		if(*recieversIterator < 19){
			recieversNeuron.removeReciever(*recieversIterator);
			n.setNeuron(recieversNeuron,loc);
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
		Neuron newNeuron = n.getNeuron(loc);
		newNeuron.addReciever(target);
		n.setNeuron(newNeuron,loc);
		if(loc > 29){
			std::forward_list<int> inputsLeft;
			for(int i = 1;i < 10;++i)
				inputsLeft.push_front(i);
			std::vector<int> locInputs = n.getInputs(loc);
			int input = -1;
			int sender = -1;
			for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator){
				if(*inputsIterator > 18 && n.getNeuron(*inputsIterator).getPulse() > 0)
					input = *inputsIterator;
				if(*inputsIterator < 10)
					sender = *inputsIterator;
			}
			getInputsLeft(n,inputsLeft,input);
			inputsLeft.remove(target - 9);
			inputsLeft.push_front(sender);
			while(!inputsLeft.empty()){
				Neuron newSender = n.getNeuron(sender);
				newSender.removeReciever(loc);
				n.setNeuron(newSender,sender);
				sender = randomLoc(inputsLeft);
				if(sender > 9)
					std::cout << "BAD\n";
				inputsLeft.remove(sender);
				newSender = n.getNeuron(sender);
				newSender.addReciever(loc);
				n.setNeuron(newSender,sender);
				int board[9];
				for(int i = 0;i < 9;++i)
					board[i] = 0;
				int opponentWinCheck = loc;
				std::vector<int> locInputs = n.getInputs(opponentWinCheck);
				int inputsSize = 0;
				for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator)
					if(n.getNeuron(*inputsIterator).getPulse() > 0 && *inputsIterator > 9)
						inputsSize++;
				while(inputsSize > 0){
					Neuron recieversNeuron = n.getNeuron(opponentWinCheck);
					for(auto recieversIterator = recieversNeuron.recieversBegin();recieversIterator != recieversNeuron.recieversEnd();++recieversIterator){
						if(*recieversIterator < 19)
							board[*recieversIterator - 10] = 1;
					}
					for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator){
						if(n.getNeuron(*inputsIterator).getPulse() > 0){
							if(*inputsIterator > 9)
								opponentWinCheck = *inputsIterator;
							else
								if(*inputsIterator != 0){
									board[*inputsIterator - 1] = -1;
								}
						}
					}						
					locInputs = n.getInputs(opponentWinCheck);
					inputsSize = 0;
					for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator)
						if(n.getNeuron(*inputsIterator).getPulse() > 0 && *inputsIterator > 9)
							inputsSize++;
				}
				Neuron recieversNeuron = n.getNeuron(opponentWinCheck);
				for(auto recieversIterator = recieversNeuron.recieversBegin();recieversIterator != recieversNeuron.recieversEnd();++recieversIterator){
					if(*recieversIterator < 19)
						board[*recieversIterator - 10] = 1;
				}
				for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator){
					if(n.getNeuron(*inputsIterator).getPulse() > 0){
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
					Neuron newNeuron = n.getNeuron(loc);
					newNeuron.removeReciever(target);
					newNeuron.addReciever(t.getOneAway(1) + 10);
					n.setNeuron(newNeuron,loc);
					gBoard[t.getOneAway(1)] = 1;
				}	
				else	
					gBoard[target - 10] = 1;
				if(t.winner() == 1)
					return 1;
				if(!t.oneAway(-1) && !t.twoAway(-1) && !t.threeAway(-1)){
					int valid = 1;
					Neuron recieversNeuron = n.getNeuron(loc);
					for(auto recieversIterator = recieversNeuron.recieversBegin();recieversIterator != recieversNeuron.recieversEnd();++recieversIterator){
						if(n.getNeuron(*recieversIterator).getPulse() > 0 && *recieversIterator > 18 && valid)
							if(recursiveMutate(n,*recieversIterator) == -1)
								valid = 0;
					}
					if(valid)
						return 1;
				}
			}
		}
		else{
			int valid = 1;
			Neuron recieversNeuron = n.getNeuron(loc);
			for(auto recieversIterator = recieversNeuron.recieversBegin();recieversIterator != recieversNeuron.recieversEnd();++recieversIterator){
				if(n.getNeuron(*recieversIterator).getPulse() > 0 && *recieversIterator > 18 && valid)
					if(recursiveMutate(n,*recieversIterator) == -1)
						valid = 0;
			}
			if(valid)
				return 1;
		}
		Neuron recieversNeuron = n.getNeuron(loc);
		for(auto recieversIterator = recieversNeuron.recieversBegin();recieversIterator != recieversNeuron.recieversEnd();++recieversIterator){
			if(*recieversIterator < 19){
				Neuron newNeuron = n.getNeuron(loc);
				newNeuron.removeReciever(*recieversIterator);
				n.setNeuron(newNeuron,loc);
				break;
			}
		}
	}
	return -1;
}

void TicTacToeTrainingMethods::getOutputsLeft(Network& n,std::forward_list<int>& outputsLeft,int loc){
	std::vector<int> locInputs = n.getInputs(loc);
	int inputsSize = 0;
	for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator)
		if(n.getNeuron(*inputsIterator).getPulse() > 0)
			inputsSize++;
	int current = loc;
	while(inputsSize > 1){
		Neuron recieversNeuron = n.getNeuron(current);
		for(auto recieversIterator = recieversNeuron.recieversBegin();recieversIterator != recieversNeuron.recieversEnd();++recieversIterator){
			if(*recieversIterator < 19){
				outputsLeft.remove(*recieversIterator - 9);
			}
		}
		for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator){
			if(n.getNeuron(*inputsIterator).getPulse() > 0){
				if(*inputsIterator < 10)
					outputsLeft.remove(*inputsIterator);
				else
					current = *inputsIterator;
			}
		}
		locInputs = n.getInputs(current);
		inputsSize = 0;
		for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator)
			if(n.getNeuron(*inputsIterator).getPulse() > 0)
				inputsSize++;
	}
	Neuron recieversNeuron = n.getNeuron(current);
	for(auto recieversIterator = recieversNeuron.recieversBegin();recieversIterator != recieversNeuron.recieversEnd();++recieversIterator){
		outputsLeft.remove(*recieversIterator - 9);
	}
	for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator){
		if(n.getNeuron(*inputsIterator).getPulse() > 0){
			if(*inputsIterator < 10){
				outputsLeft.remove(*inputsIterator);
			}
		}
	}

}

void TicTacToeTrainingMethods::getInputsLeft(Network& n,std::forward_list<int>& inputsLeft,int loc){
	Neuron recieversNeuron = n.getNeuron(loc);
	for(auto recieversIterator = recieversNeuron.recieversBegin();recieversIterator != recieversNeuron.recieversEnd();++recieversIterator){
		if(*recieversIterator > 38 && n.getNeuron(*recieversIterator).getPulse() > 0){
			std::vector<int> recieverInputs = n.getInputs(*recieversIterator);
			for(auto inputsIterator = recieverInputs.begin();inputsIterator != recieverInputs.end();++inputsIterator){
				if(*inputsIterator < 10){
					inputsLeft.remove(*inputsIterator);
				}
			}
		}
	}
	std::vector<int> locInputs = n.getInputs(loc);
	int inputsSize = 0;
	for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator)
		if(n.getNeuron(*inputsIterator).getPulse() > 0)
			inputsSize++;
	int current = loc;
	while(inputsSize > 1){
		Neuron recieversNeuron = n.getNeuron(current);
		for(auto recieversIterator = recieversNeuron.recieversBegin();recieversIterator != recieversNeuron.recieversEnd();++recieversIterator){
			if(*recieversIterator < 19){
				inputsLeft.remove(*recieversIterator - 9);
			}
		}
		for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator){
			if(n.getNeuron(*inputsIterator).getPulse() > 0){
				if(*inputsIterator < 10){
					inputsLeft.remove(*inputsIterator);
				}
				else
					current = *inputsIterator;
			}
		}
		locInputs = n.getInputs(current);
		inputsSize = 0;
		for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator)
			if(n.getNeuron(*inputsIterator).getPulse() > 0){
				inputsSize++;
			}
	}
	recieversNeuron = n.getNeuron(current);
	for(auto recieversIterator = recieversNeuron.recieversBegin();recieversIterator != recieversNeuron.recieversEnd();++recieversIterator){
		inputsLeft.remove(*recieversIterator - 9);
	}
	for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator){
		if(n.getNeuron(*inputsIterator).getPulse() > 0){
			if(*inputsIterator < 10){
				inputsLeft.remove(*inputsIterator);
			}
		}
	}

}

std::string TicTacToeTrainingMethods::newDynamicTicTacToe(Network& n){
	n = Network();
	n.dynamic = 1;
	for(int i = 0;i < 29;i++){
                n.addNeuron(Neuron());
        }
        for(int i = 0;i < 10;i++)
                n.addInput(9 - i);
        for(int i = 0;i < 9;i++)
                n.addOutput(18 - i);
        for(int i = 0;i < 10;i++){
		Neuron newNeuron = n.getNeuron(i);
		newNeuron.addReciever(i + 19);
		n.setNeuron(newNeuron,i);
		n.senders.push_front(i + 19);
		n.recievers.push_front(i + 19);
	}
        for(int i = 1;i < 10;i++){
		n.addStructure(Network::NOT,0,0,i + 19);
	}
        for(int i = 1;i < 10;i++){
		for(int j = 1;j < 10;j++){
			if(i != j)
				n.addStructure(Network::NOT,i,0,j + 19);
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
		if(n.dynamic)
			dynamicConvert(data,moveStack.top());
		moveStack.pop();
		Network tn;
		tn = networkStack.top();
		networkStack.pop();
		int move = tn.process(&data) - 10;
		if(!n.dynamic)
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
		std::vector<int> locInputs = n.getInputs(opponentWinCheck);
		int inputsSize = 0;
		for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator)
			if(n.getNeuron(*inputsIterator).getPulse() > 0 && *inputsIterator > 9)
				inputsSize++;
		while(inputsSize > 0){
			Neuron recieversNeuron = n.getNeuron(opponentWinCheck);
			for(auto recieversIterator = recieversNeuron.recieversBegin();recieversIterator != recieversNeuron.recieversEnd();++recieversIterator){
				if(*recieversIterator < 19)
					board[*recieversIterator - 10] = 1;
			}
			for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator){
				if(n.getNeuron(*inputsIterator).getPulse() > 0){
					if(*inputsIterator > 9)
						opponentWinCheck = *inputsIterator;
					else
						if(*inputsIterator != 0){
							board[*inputsIterator - 1] = -1;
						}
				}
			}						
			locInputs = n.getInputs(opponentWinCheck);
			inputsSize = 0;
			for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator)
				if(n.getNeuron(*inputsIterator).getPulse() > 0 && *inputsIterator > 9)
					inputsSize++;
		}
		Neuron recieversNeuron = n.getNeuron(opponentWinCheck);
		for(auto recieversIterator = recieversNeuron.recieversBegin();recieversIterator != recieversNeuron.recieversEnd();++recieversIterator){
			if(*recieversIterator < 19)
				board[*recieversIterator - 10] = 1;
		}
		for(auto inputsIterator = locInputs.begin();inputsIterator != locInputs.end();++inputsIterator){
			if(n.getNeuron(*inputsIterator).getPulse() > 0){
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
