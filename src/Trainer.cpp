//
//Trainer.cpp
//

#include "Trainer.h"
#include <iterator>
#include <cstdlib>
#include <fstream>
#include <forward_list>
#include "TicTacToe.h"
#include <stack>
#include "TicTacToeTrainingMethods.h"

int done = TRAINERITERATIONS;

void Trainer::dynamicTraining(int* end,Network &n){
	int start = 0;
	bool ready = false;
	n.dynamic = 1;
	int nMutations = 0;
	int nVal = evaluate(n);
	std::thread threads[NUMTHREADS];
	int* progress[NUMTHREADS];
	int** progressPointers[NUMTHREADS];
	for(int i = 0;i < NUMTHREADS;++i)
		progress[i] = &start;
	std::mutex lock;
	std::condition_variable cv;
	for(int i = 0;i < NUMTHREADS;++i){
		progressPointers[i] = &progress[i];
		threads[i] = std::thread(&Trainer::dynamicTrainingThread,this,&n,&nVal,&cv,&ready,&lock,&nMutations,progressPointers[i]);
	}
	int i = 0;
	while(i < TRAINERITERATIONS * NUMTHREADS){
		std::cout << "|";
		int bars = 100;
		int numbars = TRAINERITERATIONS * NUMTHREADS / bars;//right value is number of bars
		for(int k = 0;k <= i / numbars;++k)
			std::cout << "=";
		for(int k = 0;k < bars - (i / numbars) - 1;++k)
			std::cout << " ";
		std::cout << "|  ";
		std::cout << i << " " << nMutations << " " << nVal << "\r" << std::flush;
		std::unique_lock<std::mutex> lk(lock);
        	while(!ready) cv.wait(lk);
		ready = false;
		i = 0;
		for(int j = 0;j < NUMTHREADS;++j)
			i += *progress[j];
		if(*end){
			for(int j = 0;j < NUMTHREADS;++j)
				*progress[j] = TRAINERITERATIONS;
			*end = -1;
		}
	}
	for(int i = 0;i < NUMTHREADS;++i){
		threads[i].join();
	}
}

void Trainer::dynamicTrainingThread(Network* sourceNetwork,int* sourceVal,std::condition_variable* cv,bool* ready,std::mutex* lock,int* nMutations,int** progress){
	Network n;
	int num;
	(*lock).lock();
	n = *sourceNetwork;
	num = *nMutations;
	(*lock).unlock();
	int nVal = evaluate(n);
	int i = 0;
	(*progress) = &i;
	for(i = 0;i < TRAINERITERATIONS;++i){
		if(num < *nMutations){
			(*lock).lock();
			n = *sourceNetwork;
			num = *nMutations;
			(*lock).unlock();
			nVal = evaluate(n);
		}
		Network c;//need copy constructor
		c = n;
		int m;
		std::string deb = n.info();
		if((m = mutate(c,-1)) != -1){
				int cVal = evaluate(c);
				if(cVal == nVal && m == 2){
					n = c;
				}
				if(cVal > nVal){
					(*lock).lock();
					if(num == *nMutations){//possible adjustment to avoid better mutations being ignored
						*sourceNetwork = c;
						n = c;
						nVal = cVal;
						(*sourceVal) = cVal;
						++(*nMutations);
						++num;
						n.save("temp.ai");
					}
					(*lock).unlock();
					//(*progress) = i;
					*ready = true;
					(*cv).notify_all();
				}
		}
		*ready = true;
		(*cv).notify_all();
	}
	*progress = &done;
	*ready = true;
	(*cv).notify_all();
}


