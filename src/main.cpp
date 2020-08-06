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
#include <cstdlib>
#include <ctime>
#include <stack>

void clear(){//System specific line, must change for windows
	system("clear");
}

int locate(std::vector<std::pair<std::forward_list<int>,int> > &targetData,std::forward_list<int> data){
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

int contains(std::vector<std::pair<std::forward_list<int>,int> > &targetData,std::forward_list<int> data){
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

void convert(std::forward_list<int>&data,int *board,int turn){
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

std::string play(Network &n){
	clear();
	TicTacToe t = TicTacToe();
        while(1){
                std::cout << t.display();
                int turn = t.getTurn();
                int returnVal;
		if(turn == 1){
                        int move;
			std::cout << "make your move\n";
                        std::cin >> move;
			if(move == -1)
				return "";
                        while((returnVal = t.move(move)) == -1)
                                std::cin >> move;
                }
                else{
                        std::forward_list<int> data;
                        convert(data,t.getBoard(),t.getTurn());
			int move = n.process(&data) - 19;
			if(move == -20 || (returnVal = t.move(move)) == -1){
                                move = std::rand() % 9;
                                while((returnVal = t.move(move)) == -1)
                                        move = std::rand() % 9;
			}
		}
		if(returnVal == 1)
			t.reset();
		clear();
        }
	
}

std::string train(Network &n,std::vector<std::pair<std::forward_list<int>,int> > &targetData, int random){
	clear();
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
                convert(data,t.getBoard(),t.getTurn());
                int move = -20;
                        if(contains(targetData,data)){
                                move = locate(targetData,data) - 19;
                                t.move(move);
                        }       
                        else{   
                                std::cout << "make your move\n";
                                std::cin >> move;
                                if(move == -1)
                                        return "";
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
                clear();
	}
	//return value
	return "trained";
}

std::string newTicTacToe(Network& n){
	n = Network();
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
	return "Default TicTacToe Network loaded";
}

int main(){
	std::srand(std::time(0));
        Network n = Network();
	std::vector<std::pair<std::forward_list<int>,int> > targetData;
	std::string filename;
	std::string buffer = "";
	std::string message = "";
	do{
		if(buffer == "1"){
			std::cout << "Enter your filename: ";
			std::cin >> filename;
			message = "Loaded " + filename;
			n.load(filename);
		}
		if(buffer == "2"){
			message = newTicTacToe(n);
			targetData.clear();
		}
		if(buffer == "3")
			message = train(n,targetData,0);
		if(buffer == "4")
			message = train(n,targetData,1);
		if(buffer == "5")
			message = play(n);
		if(buffer == "6")
			message = n.info();
		if(buffer == "7"){
			std::cout << "Enter your filename: ";
                        std::cin >> filename;
			message = "Saved " + filename;
			n.save(filename);
		}
		clear();
		std::string menu = "Menu: " + message + "\n\t1. Load \n\t2. New\n\t3. Train\n\t4. Train with random moves\n\t5. Play\n\t6. Display Network\n\t7. Save\n\t-1. Exit\n";
		std::cout << menu << "\n";
		std::cin >> buffer;

	}
	while(buffer != "-1");
}
