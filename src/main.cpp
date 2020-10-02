//
//main.cpp
//

#include "TicTacToe.h"
#include "Trainer.h"
#include "TicTacToeTrainingMethods.h"
#include "Network.h"
#include <utility>
#include <forward_list>
#include <cstdlib>
#include <thread>

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
			if(n.dynamic)
                        	TicTacToeTrainingMethods::dynamicConvert(data,move);
			int aiMove = n.process(&data) - 10;
			if(!n.dynamic)
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


int main(){
	std::srand(std::time(0));
        Network n;
	TicTacToeTrainingMethods::newDynamicTicTacToe(n);
	int end = 0;
	std::string filename;
	std::string buffer = "";
	std::string message = "";

	Trainer TicTacToeTrainer = Trainer(&TicTacToeTrainingMethods::evaluate,&TicTacToeTrainingMethods::mutate);

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
			message = TicTacToeTrainingMethods::newDynamicTicTacToe(n);
		}
		if(buffer == "3"){
			std::thread(&Trainer::dynamicTraining ,&TicTacToeTrainer ,&end ,std::ref(n)).detach();
			message = "";
		}
		if(buffer == "4")
			message = play(n);
		if(buffer == "5")
			message = n.info();
		if(buffer == "6"){
			std::cout << "Enter your filename: ";
                        std::cin >> filename;
			message = "Saved " + filename;
			n.save(filename);
		}
		if(buffer == "e"){
			end = 1;
			while(end != -1){

			}
			end = 0;
		}
		system("clear");
		std::string menu = "Menu: " + message + "\n\t1. Load \n\t2. New\n\t3. Train\n\t4. Play\n\t5. Display Network\n\t6. Save\n\t-1. Exit\n";
		std::cout << menu << "\n";
		std::cin >> buffer;

	}
	while(buffer != "-1");
	exit(0);
}
