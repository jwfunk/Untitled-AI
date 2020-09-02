CXX=gcc-9
CXXFLAGS=-Wall
CXXFLAGS+=-std=c++11
CXXFLAGS+=-ggdb3
#CXXFLAGS+=-g
CXXFLAGS+=-lstdc++
all:main
src/Neuron.o: src/Neuron.cpp src/Neuron.h

src/Network.o: src/Network.cpp src/Network.h

src/Trainer.o: src/Trainer.cpp src/Trainer.h src/Network.h

src/TicTacToe.o: src/TicTacToe.cpp src/TicTacToe.h

clean:
	-rm src/Neuron.o
	-rm src/Network.o
	-rm src/Trainer.o
	-rm src/TicTacToe.o
	-rm main.exe

main: src/Neuron.o src/Network.o src/Trainer.o src/TicTacToe.o
	$(CXX) $(CXXFLAGS) -o main.exe src/main.cpp src/Network.o src/Neuron.o src/Trainer.o src/TicTacToe.o
