//
//main.cpp
//

#include "TicTacToe.h"
#include "Trainer.h"
#include "Network.h"
#include <vector>
#include <utility>
#include <forward_list>
#include <fstream>

int locate(std::vector<std::pair<std::forward_list<int>,int> > &targetData,std::forward_list<int> data){
	for(auto it = targetData.begin();it != targetData.end();++it){
                auto ittt = data.begin();
                int match = 1;
                for(auto itt = (*it).first.begin();itt != (*it).first.end();++itt){
                        if(*itt != *ittt)
                                match = 0;
                        ++ittt;
                }
                if(match)
                        return (*it).second;
        }
        return -1;
}

int contains(std::vector<std::pair<std::forward_list<int>,int> > &targetData,std::forward_list<int> data){
	for(auto it = targetData.begin();it != targetData.end();++it){
		auto ittt = data.begin();
		int match = 1;
		for(auto itt = (*it).first.begin();itt != (*it).first.end();++itt){
			if(*itt != *ittt)
				match = 0;
			++ittt;
		}
		if(match)
			return 1;
	}
	return 0;
}

void convert(std::forward_list<int>&data,int *board,int t){
	int z = 1;
	int b;
	if(t == 1)
		b = -1;
	else
		b = 1;
	for(int i = 8;i >= 0;i--){
		if(board[i] == b){
			data.push_front(1);
			z = 0;
		}
		else{
			data.push_front(0);
		}
	}
	for(int i = 8;i >= 0;i--){
                if(board[i] == t){
                        data.push_front(1);
                        z = 0;
                }
                else{
                        data.push_front(0);
                }
        }
	if(z)
		data.push_front(1);
	else
		data.push_front(0);
}

int main(){
        Network n = Network();
	
	for(int i = 0;i < 19;i++){
                n.addNeuron(Neuron());
        }
        for(int i = 0;i < 19;i++)
                n.addInput(18 - i);
        for(int i = 0;i < 9;i++){
                n.addNeuron(Neuron());
        }
        for(int i = 0;i < 9;i++)
                n.addOutput(27 - i);

        std::vector<std::pair<std::forward_list<int>,int> > targetData;
	std::vector<std::pair<std::forward_list<int>,int> > nextData;
        TicTacToe t = TicTacToe();
        while(1){
                std::cout << t.display();
                int turn = t.getTurn();
                if(turn == 1){
                        std::forward_list<int> data;
                        convert(data,t.getBoard(),t.getTurn());
                        std::cout << "make your move\n";
                        int move;
                        std::cin >> move;
                        if(!contains(targetData,data)){
                                std::pair<std::forward_list<int>,int> p;
                                p.first = data;
                                p.second = move + 19;
                                targetData.push_back(p);
				nextData.push_back(p);
				Trainer::trainPrecisionLearning(n,nextData);
				nextData.clear();
                        }
                        if(t.move(move) == 1)
                                t.reset();
                }
                else{
			std::forward_list<int> data;
                        convert(data,t.getBoard(),t.getTurn());
				int move = n.process(&data) - 19;
				if(move == -1 || t.move(move) == -1){
					move = std::rand() % 9;
                	        	int r = 0;
                	        	if((r = t.move(move)) == -1)
                	        	        move = std::rand() % 9;
                	        	if(r == 1)
                	        	        t.reset();
			}
        	}
	}
}
