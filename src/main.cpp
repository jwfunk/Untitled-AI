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
#include <signal.h>
#include <thread>

void ctrlCHandler(int i){
	std::cout << "Caught signal " << i << "\n";
	exit(0);
}

std::string play(Network &n){
	system("clear");
	TicTacToe t = TicTacToe();
	int move = -1;
        while(1){
                std::cout << t.display();
                int turn = t.getTurn();
                int returnVal;
		if(turn == 1){
			std::cout << "make your move\n";
                        std::cin >> move;
			if(move == -1){
				n.clear();
				return "";
			}
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
			if(!n.getDynamic())
				aiMove -= 10;
			if((returnVal = t.move(aiMove)) == -1){
				std::cout << aiMove << "\n";
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
		system("clear");
        }
	
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
	int end = 0;
	std::vector<std::pair<std::forward_list<int>,int> > targetData;
	std::string filename;
	std::string buffer = "";
	std::string message = "";
	struct sigaction sigIntHandler;

	sigIntHandler.sa_handler = ctrlCHandler;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;

	sigaction(SIGINT, &sigIntHandler, NULL);


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
			message = Trainer::newDynamicTicTacToe(n);
			targetData.clear();
		}
		if(buffer == "8")
			message = newStaticTicTacToe(n);
		if(buffer == "3")
			Trainer::staticTraining(n,targetData);
		if(buffer == "4"){
			std::thread(Trainer::dynamicTraining,&end,std::ref(n),500000).detach();
			message = "";
		}
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
		if(buffer == "9")
			message = std::to_string(Trainer::tevaluate(n));
		if(buffer == "e"){
			end = 1;
			while(end != -1){

			}
			end = 0;
		}
		system("clear");
		std::string menu = "Menu: " + message + "\n\t1. Load \n\t2. New\n\t3. Train\n\t5. Play\n\t6. Display Network\n\t7. Save\n\t-1. Exit\n";
		std::cout << menu << "\n";
		std::cin >> buffer;

	}
	while(buffer != "-1");
}
