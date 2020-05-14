CXX=g++
CXXFLAGS=-Wall
CXXFLAGS+=-std=c++11
CXXFLAGS+=-ggdb3
all:neuron network
../../src/Neuron.o: ../../src/Neuron.cpp ../../src/Neuron.h

../../src/Network.o: ../../src/Network.cpp ../../src/Network.h
clean:
	-rm ../../src/Neuron.o
	-rm neuronUnitTests
	-rm ../../src/Network.o
	-rm networkUnitTests

neuron: NeuronUnitTests.cpp ../../src/Neuron.o
	$(CXX) $(CXXFLAGS) -o neuronUnitTests NeuronUnitTests.cpp ../../src/Neuron.o

network: NetworkUnitTests.cpp ../../src/Network.o
	$(CXX) $(CXXFLAGS) -o networkUnitTests NetworkUnitTests.cpp ../../src/Network.o ../../src/Neuron.o
