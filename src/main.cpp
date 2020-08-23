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

std::string play(Network &n){
	clear();
	TicTacToe t = TicTacToe();
	int move = -1;
        while(1){
                std::cout << t.display();
                int turn = t.getTurn();
                int returnVal;
		if(turn == 1){
			std::cout << "make your move\n";
                        std::cin >> move;
			if(move == -1)
				return "";
                        while((returnVal = t.move(move)) == -1)
                                std::cin >> move;
                }
                else{
                        std::forward_list<int> data;
			if(n.getDynamic())
                        	Trainer::dynamicConvert(data,move);
			else
				Trainer::staticConvert(data,t.getBoard(),t.getTurn());
			int aiMove = n.process(&data) - 10;
			if(move == -20 || (returnVal = t.move(aiMove)) == -1){
                                aiMove = std::rand() % 9;
                                while((returnVal = t.move(aiMove)) == -1)
                                        aiMove = std::rand() % 9;
			}
		}
		if(returnVal == 1){
			t.reset();
			n.clear();
			move = -1;
		}
		clear();
        }
	
}


std::string newDynamicTicTacToe(Network& n){
	n = Network();
	n.setDynamic(1);
	for(int i = 0;i < 19;i++){
                n.addNeuron(Neuron());
        }
        for(int i = 0;i < 10;i++)
                n.addInput(9 - i);
        for(int i = 0;i < 9;i++)
                n.addOutput(18 - i);
	return "Default Dynamic TicTacToe Network loaded";
}

std::string newStaticTicTacToe(Network& n){
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
	return "Default Static TicTacToe Network loaded";
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
			if(n.load(filename) == 0)
				message = "Loaded " + filename;
			else
				message = "Could not load " + filename;
		}
		if(buffer == "2"){
			message = newDynamicTicTacToe(n);
			targetData.clear();
		}
		if(buffer == "8")
			message = newStaticTicTacToe(n);
		if(buffer == "3")
			Trainer::staticTraining(n,targetData);
		if(buffer == "4")
			Trainer::dynamicTraining(n,1000000);
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
		std::string menu = "Menu: " + message + "\n\t1. Load \n\t2. New\n\t3. Train\n\t5. Play\n\t6. Display Network\n\t7. Save\n\t-1. Exit\n";
		std::cout << menu << "\n";
		std::cin >> buffer;

	}
	while(buffer != "-1");
}
