//
//TicTacToe.h
//
//Header file for TicTacToe class
//TicTacToe objects consist of:
//board: keeps track of player's moves
//turn: keeps track of which player's turn it is
#ifndef TICTACTOE
#define TICTACTOE

#include <string>

class TicTacToe{
	
	public:
	
	//constructors
	TicTacToe();

	int move(int);

	int getTurn() {return turn;}

	int * getBoard() {return board;}

	std::string display();

	void swap();

	void reset();

	TicTacToe& operator=(const TicTacToe &);

	int winner();

	private:
	
	int board[9];
	int turn{1};
};

#endif
