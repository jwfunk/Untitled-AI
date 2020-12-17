//
//StockTrainingMethods.cpp
//

#include "StockTrainingMethods.h"
#include <fstream>
#include <sstream>
#include <stdlib.h>
StockTrainingMethods::StockTrainingMethods(): Trainer(0){
	std::fstream fin;

	fin.open("data/AAPL.csv", std::ios::in);

	std::string temp;

	while(fin >> temp){
		std::stringstream ss(temp);
		std::string val;
		std::getline(ss,val,',');
		openingVals.push_back(std::atof(val.c_str()));
		std::getline(ss,val,',');
		closingVals.push_back(std::atof(val.c_str()));
	}
}

std::string StockTrainingMethods::newDynamicTicTacToe(Network& n){
	n = Network();
	for(int i = 0;i < 19;++i)
		n.addNeuron(Neuron());
	for(int i = 0;i < 17;++i){
		n.addInput(i);
	}
	for(int i = 17;i < 19;++i){
		n.addOutput(i);
		n.recievers.push_front(i);
	}
	int binaryVals[16] = {
	32768,
	16384,
	8192,
	4096,
	2048,
	1024,
	512,
	256,
	128,
	64,
	32,
	16,
	8,
	4,
	2,
	1
	};
	for(int i = 1; i < 17;++i){
		Neuron newNeuron = Neuron(1,binaryVals[i - 1]);
		n.addNeuron(newNeuron);
		newNeuron = n.getNeuron(i);
		newNeuron.addReciever(i + 18);
		n.senders.push_front(i + 18);
		n.setNeuron(newNeuron,i);
	}
	for(int i = 1;i < 17;++i){
		Neuron newNeuron = Neuron(2,binaryVals[i - 1] * 2 * -1);
		n.addNeuron(newNeuron);
		newNeuron = n.getNeuron(i);
		newNeuron.addReciever(i + 34);
		n.setNeuron(newNeuron,i);
		newNeuron = n.getNeuron(0);
		newNeuron.addReciever(i + 34);
		n.senders.push_front(i + 34);
		n.setNeuron(newNeuron,0);
	}
	for(int i = 1; i < 17;++i){
		Neuron newNeuron = Neuron(1,binaryVals[i - 1] * 2 * -1);
		n.addNeuron(newNeuron);
		newNeuron = n.getNeuron(i);
		newNeuron.addReciever(i + 50);
		n.senders.push_front(i + 50);
		n.setNeuron(newNeuron,i);
	}
	for(int i = 1;i < 17;++i){
		Neuron newNeuron = Neuron(2,binaryVals[i - 1]);
		n.addNeuron(newNeuron);
		newNeuron = n.getNeuron(i);
		newNeuron.addReciever(i + 66);
		n.setNeuron(newNeuron,i);
		newNeuron = n.getNeuron(0);
		newNeuron.addReciever(i + 66);
		n.senders.push_front(i + 66);
		n.setNeuron(newNeuron,0);
	}
	return "Default stock network loaded";
}

int StockTrainingMethods::evaluate(Network &n,Network &c){
	double nscore = 0;
	double cscore = 0;
	int good = 0;
	int size = closingVals.size();
	for(int i = 0;i < 10;++i){
		int loc = std::rand() % (size - 61);
		for(int j = 0;j < 30;++j){
			n.process(convert(openingVals[loc],closingVals[loc]));
			c.process(convert(openingVals[loc],closingVals[loc]));
			++loc;
		}
		for(int j = 0;j < 30;++j){
			int action = n.process(convert(openingVals[loc],closingVals[loc]));
			++loc;
			if(action == 17){//buy
				good = 1;
				if(closingVals[loc] > openingVals[loc + 1])
					++nscore;
				if(closingVals[loc] < openingVals[loc + 1])
					--nscore;
			}
			if(action == 18){//sell
				if(closingVals[loc] > openingVals[loc + 1])
					--nscore;
				if(closingVals[loc] < openingVals[loc + 1])
					++nscore;
			}
			--loc;
			action = c.process(convert(openingVals[loc],closingVals[loc]));
			++loc;
			if(action == 17){//buy
				if(closingVals[loc] > openingVals[loc + 1])
					++cscore;
				if(closingVals[loc] < openingVals[loc + 1])
					--cscore;
			}
			if(action == 18){//sell
				if(closingVals[loc] > openingVals[loc + 1])
					--cscore;
				if(closingVals[loc] < openingVals[loc + 1])
					++cscore;
			}
		}
		n.clear();
		c.clear();
	}
	if(!good)
		return -1;
	if(nscore > cscore)
		return 1;
	if(cscore > nscore)
		return -1;
	return 0;
}

std::forward_list<int> StockTrainingMethods::convert(double val1,double val2){
	std::forward_list<int> data;
	double percentage = (val2 - val1) / val1;
	double binaryVals[16] = {
	327.68,
	163.84,
	81.92,
	40.96,
	20.48,
	10.24,
	5.12,
	2.56,
	1.28,
	0.64,
	0.32,
	0.16,
	0.8,
	0.4,
	0.2,
	0.1
	};
	if(percentage < 0){
		data.push_front(1);
		percentage *= -1;
	}
	else
		data.push_front(0);
	for(int i = 0;i < 16;++i){
		if(percentage > binaryVals[i]){
			data.push_front(1);
			percentage -= binaryVals[i];
		}
		else
			data.push_front(0);
	}
	return data;
}

int StockTrainingMethods::choose(Network& n){
	if(n.notTargets.empty())		
		return 0;
	int numChoices = 6;
	int choices[] = {10,10,10,10,10,10};
	if(n.nextLocation() == -1){
		return -1;//fix later
	}
	int randSize = 0;
	for(int i = 0;i < numChoices;++i)
		randSize += choices[i];	
	int choice = std::rand() % randSize;
	for(int i = 0;i < numChoices;++i){
		choice -= choices[i];
		if(choice < 0)
			return i;
	}	
	return -1;
}

int StockTrainingMethods::mutate(Network &n,int choice){
	for(int i = 0;i < NMUTATIONS;++i){
		if(choice == -1)
			choice = choose(n);
		switch(choice){
			case 0:
			{
				Neuron counter = Neuron(std::rand() % 1000,1);
				int loc = n.nextLocation();
				n.addNeuron(counter);
				n.notTargets.push_front(loc);
				int ASN = std::rand() % 3;//Add Subtract or None
				if(ASN == 1){
					for(int i = 19;i < 35;++i){
						Neuron newNeuron = n.getNeuron(i);
						newNeuron.addReciever(loc);
						n.setNeuron(newNeuron,i);
					}
					ASN = std::rand() % 3;
					if(ASN == 1){
						for(int i = 67;i < 83;++i){
							Neuron newNeuron = n.getNeuron(i);
							newNeuron.addReciever(loc);
							n.setNeuron(newNeuron,i);
						}
					}
					if(ASN == 2){
						for(int i = 51;i < 67;++i){
							Neuron newNeuron = n.getNeuron(i);
							newNeuron.addReciever(loc);
							n.setNeuron(newNeuron,i);
						}
					}
					ASN = 1;
				}
				if(ASN == 2){
					for(int i = 51;i < 67;++i){
						Neuron newNeuron = n.getNeuron(i);
						newNeuron.addReciever(loc);
						n.setNeuron(newNeuron,i);
					}
				}
				if(ASN == 0 || ASN == 2){
					for(int i = 67;i < 83;++i){
						Neuron newNeuron = n.getNeuron(i);
						newNeuron.addReciever(loc);
						n.setNeuron(newNeuron,i);
					}
				}
				if(n.notTargets.empty() || std::rand() % 2){
					Neuron newNeuron = n.getNeuron(loc);
					newNeuron.addReciever(randomLoc(n.recievers));
					n.setNeuron(newNeuron,loc);
				}
				else{
					int reciever = randomLoc(n.notTargets);
					if(reciever == loc)	
						reciever = randomLoc(n.recievers);
					if(std::rand() % 2){
						n.addStructure(Network::PAND,loc,0,reciever);	
					}
					else
						n.addStructure(Network::POR,loc,0,reciever);	
				}
			}
			case 1:
			{
				int sender1 = randomLoc(n.notTargets);
				int sender2 = randomLoc(n.notTargets);
				int reciever = randomLoc(n.recievers);
				n.addStructure(Network::AND,sender1,sender2,reciever);
			}	
			case 2:
			{
				int sender1 = randomLoc(n.notTargets);
				int sender2 = randomLoc(n.notTargets);
				int reciever = randomLoc(n.recievers);
				n.addStructure(Network::OR,sender1,sender2,reciever);	
			}
			case 3:
			{
				int sender = randomLoc(n.notTargets);
				int reciever = randomLoc(n.recievers);
				n.addStructure(Network::PAND,sender,0,reciever);
			}
			case 4:
			{
				int sender = randomLoc(n.notTargets);
				int reciever = randomLoc(n.recievers);
				n.addStructure(Network::POR,sender,0,reciever);
			}	
			case 5:
			{
				int sender1 = randomLoc(n.notTargets);
				int sender2 = randomLoc(n.notTargets);
				int reciever = randomLoc(n.recievers);
				n.addStructure(Network::XOR,sender1,sender2,reciever);	
			}
		}
	}
	return 0;
}

int StockTrainingMethods::randomLoc(std::forward_list<int>& usedLocs){
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
double StockTrainingMethods::evaluate(Network &n){
	double nscore = 0;
	int stock = 0;
	double money = 1000;
	int size = closingVals.size();
	int neverBuy = 1;
	for(int i = 0;i < 100;++i){
		money = 1000;
		stock = 0;
		int loc = std::rand() % (size - 61);
		for(int j = 0;j < 30;++j){
			n.process(convert(openingVals[loc],closingVals[loc]));
			++loc;
		}
		for(int j = 0;j < 30;++j){
			int action = n.process(convert(openingVals[loc],closingVals[loc]));
			++loc;
			if(action == 17){//buy
				stock += money / openingVals[loc + 1];
				while(money > openingVals[loc + 1])
					money -= openingVals[loc + 1];
				neverBuy = 0;
			}
			if(action == 18){//sell
				money += stock * openingVals[loc + 1];
				stock = 0;
			}
		}
		n.clear();
		money += stock * openingVals[loc + 1];
		nscore += money - 1000;
	}
	return nscore;
}
