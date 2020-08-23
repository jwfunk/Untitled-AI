
//TicTacToe.cpp

#include "TicTacToe.h"

TicTacToe::TicTacToe(){
	for(int i = 0;i < 9;i++)
		board[i] = 0;
}

int TicTacToe::winner(){
	if(board[0] != 0 && board[0] == board[1] && board[0] == board[2])
                return board[0];
        if(board[3] != 0 && board[3] == board[4] && board[3] == board[5])
		return board[3];
        if(board[6] != 0 && board[6] == board[7] && board[6] == board[8])
                return board[6];
        if(board[0] != 0 && board[0] == board[3] && board[0] == board[6])
                return board[0];
        if(board[1] != 0 && board[1] == board[4] && board[1] == board[7])
                return board[1];
        if(board[2] != 0 && board[2] == board[5] && board[2] == board[8])
                return board[2];
        if(board[0] != 0 && board[0] == board[4] && board[0] == board[8])
                return board[0];
        if(board[6] != 0 && board[6] == board[4] && board[6] == board[2])
                return board[6];
	return 0;
}

int TicTacToe::move(int m){
	if(m < 0 || m > 8)
		return -1;
	if(board[m] != 0)
		return -1;
	else{
		board[m] = turn;
	}
	if(turn == 1)
                turn = -1;
        else
                turn = 1;	
	if(board[0] != 0 && board[0] == board[1] && board[0] == board[2])
		return 1;
	if(board[3] != 0 && board[3] == board[4] && board[3] == board[5])
                return 1;
	if(board[6] != 0 && board[6] == board[7] && board[6] == board[8])
                return 1;
	if(board[0] != 0 && board[0] == board[3] && board[0] == board[6])
                return 1;
	if(board[1] != 0 && board[1] == board[4] && board[1] == board[7])
                return 1;
	if(board[2] != 0 && board[2] == board[5] && board[2] == board[8])
                return 1;
	if(board[0] != 0 && board[0] == board[4] && board[0] == board[8])
                return 1;
	if(board[6] != 0 && board[6] == board[4] && board[6] == board[2])
                return 1;
	int z = 0;
	for(int i = 0;i < 9;i++){
		if(board[i] == 0)
			z = 1;
	}
	if(z)
		return 0;
	return 1;
}

std::string TicTacToe::display(){
	std::string result;
	result = std::to_string(board[0]) + " " + std::to_string(board[1]) + " " + std::to_string(board[2]) + "\n" + std::to_string(board[3]) + " " + std::to_string(board[4]) + " " + std::to_string(board[5]) + "\n" + std::to_string(board[6]) + " " + std::to_string(board[7]) + " " + std::to_string(board[8]) + "\n";
	return result;
}

void TicTacToe::reset(){
	for(int i = 0;i < 9;i++)
                board[i] = 0;
}

void TicTacToe::swap(){
	if(turn == 1)
                turn = -1;
        else
                turn = 1;
}

TicTacToe& TicTacToe::operator=(const TicTacToe &t){
	for(int i = 0;i < 9;i++)
		board[i] = t.board[i];
	turn = t.turn;
	return *this;
}

int TicTacToe::oneAway(int turn){
	
	if(board[0] == 0 && turn == board[1] && turn == board[2])
		return 1;
	if(board[0] == turn && 0 == board[1] && turn == board[2])
		return 1;
	if(board[0] == turn && turn == board[1] && 0 == board[2])
		return 1;

	if(board[3] == 0 && turn == board[4] && turn == board[5])
                return 1;
	if(board[3] == turn && 0 == board[4] && turn == board[5])
                return 1;
	if(board[3] == turn && turn == board[4] && 0 == board[5])
                return 1;

	if(board[6] == 0 && turn == board[7] && turn == board[8])
                return 1;
	if(board[6] == turn && 0 == board[7] && turn == board[8])
                return 1;
	if(board[6] == turn && turn == board[7] && 0 == board[8])
                return 1;

	if(board[0] == 0 && turn == board[3] && turn == board[6])
                return 1;
	if(board[0] == turn && 0 == board[3] && turn == board[6])
                return 1;
	if(board[0] == turn && turn == board[3] && 0 == board[6])
                return 1;

	if(board[1] == 0 && turn == board[4] && turn == board[7])
                return 1;
	if(board[1] == turn && 0 == board[4] && turn == board[7])
                return 1;
	if(board[1] == turn && turn == board[4] && 0 == board[7])
                return 1;

	if(board[2] == 0 && turn == board[5] && turn == board[8])
                return 1;
	if(board[2] == turn && 0 == board[5] && turn == board[8])
                return 1;
	if(board[2] == turn && turn == board[5] && 0 == board[8])
                return 1;

	if(board[0] == 0 && turn == board[4] && turn == board[8])
                return 1;
	if(board[0] == turn && 0 == board[4] && turn == board[8])
                return 1;
	if(board[0] == turn && turn == board[4] && 0 == board[8])
                return 1;

	if(board[6] == 0 && turn == board[4] && turn == board[2])
                return 1;
	if(board[6] == turn && 0 == board[4] && turn == board[2])
                return 1;
	if(board[6] == turn && turn == board[4] && 0 == board[2])
                return 1;

        return 0;
}

int TicTacToe::getOneAway(int turn){

	if(board[0] == 0 && turn == board[1] && turn == board[2])
		return 0;
	if(board[0] == turn && 0 == board[1] && turn == board[2])
		return 1;
	if(board[0] == turn && turn == board[1] && 0 == board[2])
		return 2;

	if(board[3] == 0 && turn == board[4] && turn == board[5])
                return 3;
	if(board[3] == turn && 0 == board[4] && turn == board[5])
                return 4;
	if(board[3] == turn && turn == board[4] && 0 == board[5])
                return 5;

	if(board[6] == 0 && turn == board[7] && turn == board[8])
                return 6;
	if(board[6] == turn && 0 == board[7] && turn == board[8])
                return 7;
	if(board[6] == turn && turn == board[7] && 0 == board[8])
                return 8;

	if(board[0] == 0 && turn == board[3] && turn == board[6])
                return 0;
	if(board[0] == turn && 0 == board[3] && turn == board[6])
                return 3;
	if(board[0] == turn && turn == board[3] && 0 == board[6])
                return 6;

	if(board[1] == 0 && turn == board[4] && turn == board[7])
                return 1;
	if(board[1] == turn && 0 == board[4] && turn == board[7])
                return 4;
	if(board[1] == turn && turn == board[4] && 0 == board[7])
                return 7;

	if(board[2] == 0 && turn == board[5] && turn == board[8])
                return 2;
	if(board[2] == turn && 0 == board[5] && turn == board[8])
                return 5;
	if(board[2] == turn && turn == board[5] && 0 == board[8])
                return 8;

	if(board[0] == 0 && turn == board[4] && turn == board[8])
                return 0;
	if(board[0] == turn && 0 == board[4] && turn == board[8])
                return 4;
	if(board[0] == turn && turn == board[4] && 0 == board[8])
                return 8;

	if(board[6] == 0 && turn == board[4] && turn == board[2])
                return 6;
	if(board[6] == turn && 0 == board[4] && turn == board[2])
                return 4;
	if(board[6] == turn && turn == board[4] && 0 == board[2])
                return 2;

        return -1;
}
