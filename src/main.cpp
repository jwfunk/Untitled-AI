//
//main.cpp
//

#include "TicTacToe.h"
#include "Trainer.h"
#include "StockTrainingMethods.h"
#include "Network.h"
#include <utility>
#include <forward_list>
#include <cstdlib>
#include <thread>



int main(){
	std::srand(std::time(0));
        Network n;

	int end = 0;
	std::string filename;
	std::string buffer = "";
	std::string message = "";

	StockTrainingMethods StockTrainer = StockTrainingMethods();

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
			message = StockTrainingMethods::newDynamicTicTacToe(n);
		}
		if(buffer == "3"){
			std::thread(&Trainer::dynamicTraining ,&StockTrainer ,&end ,std::ref(n)).detach();
			message = "";
		}
		if(buffer == "4")
			std::cout << StockTrainer.evaluate(n) << "\n";
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
