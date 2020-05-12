CXX=g++
CXXFLAGS=-Wall
CXXFLAGS+=-std=c++11
all:build
../../src/Neuron.o: ../../src/Neuron.cpp ../../src/Neuron.h
clean:
	-rm ../../src/Neuron.o
	-rm test

build: NeuronUnitTests.cpp ../../src/Neuron.o
	$(CXX) $(CXXFLAGS) -o test NeuronUnitTests.cpp ../../src/Neuron.o
