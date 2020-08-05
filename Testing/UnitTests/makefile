CXX=gcc-9
CXXFLAGS=-Wall
CXXFLAGS+=-std=c++11
CXXFLAGS+=-ggdb3
#CXXFLAGS+=-g
CXXFLAGS+=-lstdc++
all:neuron network trainer
../../src/Neuron.o: ../../src/Neuron.cpp ../../src/Neuron.h

../../src/Network.o: ../../src/Network.cpp ../../src/Network.h

../../src/Trainer.o: ../../src/Trainer.cpp ../../src/Trainer.h

../../src/TicTacToe.o: ../../src/TicTacToe.cpp ../../src/TicTacToe.h
clean:
	-rm ../../src/Neuron.o
	-rm neuronUnitTests
	-rm ../../src/Network.o
	-rm networkUnitTests
	-rm ../../src/Trainer.o
	-rm trainerUnitTests
	-rm ../../src/TicTacToe.o

neuron: NeuronUnitTests.cpp ../../src/Neuron.o
	$(CXX) $(CXXFLAGS) -o neuronUnitTests NeuronUnitTests.cpp ../../src/Neuron.o

network: NetworkUnitTests.cpp ../../src/Network.o
	$(CXX) $(CXXFLAGS) -o networkUnitTests NetworkUnitTests.cpp ../../src/Network.o ../../src/Neuron.o

trainer: TrainerUnitTests.cpp ../../src/Trainer.o ../../src/TicTacToe.o
	$(CXX) $(CXXFLAGS) -o trainerUnitTests TrainerUnitTests.cpp ../../src/Network.o ../../src/Neuron.o ../../src/Trainer.o ../../src/TicTacToe.o
