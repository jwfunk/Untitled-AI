
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

TicTacToe::TicTacToe(const TicTacToe & t){
	for(int i = 0;i < 9;i++)
                board[i] = t.board[i];
        turn = t.turn;
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

int TicTacToe::threeAway(int turn){
        int cases = 24;
        int data[cases][7] = {
        {5,7,0,3,4,6,8},
        {5,7,0,1,2,4,8},
        {3,7,2,4,5,6,8},
        {3,7,2,0,1,4,6},
        {1,5,6,0,2,3,4},
        {1,5,6,2,4,7,8},
        {1,3,8,0,2,4,5},
        {1,3,8,0,4,6,3},
        {5,0,7,3,4,6,8},
        {5,0,7,1,2,4,8},
        {3,2,7,4,5,6,8},
        {3,2,7,0,1,4,6},
        {1,6,5,0,2,3,4},
        {1,6,5,2,4,7,8},
        {1,8,3,0,2,4,5},
        {1,8,3,0,4,6,3},
        {0,7,5,3,4,6,8},
        {0,7,5,1,2,4,8},
        {2,7,3,4,5,6,8},
        {2,7,3,0,1,4,6},
        {6,5,1,0,2,3,4},
        {6,5,1,2,4,7,8},
        {8,3,1,0,2,4,5},
        {8,3,1,0,4,6,3}
        };
        for(int i = 0;i < cases;++i){
                if(board[data[i][0]] == turn && board[data[i][1]] == turn && board[data[i][2]] == 0 && board[data[i][3]] == 0 && board[data[i][4]] == 0 && board[data[i][4]] == 0 && board[data[i][5]] == 0 && board[data[i][6]] == 0){
                        return 1;
                }
        }
        return 0;
}

int TicTacToe::twoAway(int turn){
	int oTurn;
	if(turn == 1)
		oTurn = -1;
	else
		oTurn = 1;
	int cases = 80;
	int data[cases][5] = {
	{0,1,4,7,8},
	{0,3,4,5,8},
	{0,2,4,6,8},
	{0,2,6,3,4},
	{0,2,8,4,5},
	{0,6,4,2,8},
	{0,6,2,1,4},
	{0,6,8,4,7},
	{0,4,1,2,7},
	{0,4,2,6,1},
	{0,4,3,5,6},
	{0,4,6,2,3},
	{0,8,2,1,5},
	{0,8,6,3,7},
	{0,7,4,1,8},
	{0,7,8,4,6},
	{0,7,6,3,8},
	{0,5,4,3,8},
	{0,5,2,1,8},
	{0,5,8,4,2},
	{8,7,4,0,1},
	{8,5,4,0,3},
	{8,6,4,0,2},
	{8,6,0,3,4},
	{8,6,2,4,5},
	{8,2,4,0,6},
	{8,2,0,1,4},
	{8,2,6,4,7},
	{8,4,5,2,3},
	{8,4,2,5,6},
	{8,4,7,1,6},
	{8,4,6,7,2},
	{8,1,0,2,4},
	{8,1,2,0,5},
	{8,1,4,0,7},
	{8,3,0,4,6},
	{8,3,4,0,5},
	{8,3,6,0,7},
	{2,1,4,6,7},
	{2,5,4,3,6},
	{2,4,0,1,8},
	{2,4,1,0,7},
	{2,4,5,3,8},
	{2,4,8,0,5},
	{2,6,0,1,3},
	{2,6,8,5,7},
	{2,3,0,1,6},
	{2,3,4,5,6},
	{2,3,6,0,4},
	{2,7,4,1,6},
	{2,7,6,4,8},
	{2,7,8,5,6},
	{6,3,4,2,5},
	{6,7,4,1,2},
	{6,4,0,3,8},
	{6,4,3,0,5},
	{6,4,7,8,1},
	{6,4,8,7,0},
	{6,1,0,2,3},
	{6,1,2,0,4},
	{6,1,4,2,7},
	{6,5,2,4,8},
	{6,5,4,2,3},
	{6,5,8,2,7},
	{4,1,0,2,8},
	{4,1,2,0,6},
	{4,5,2,6,8},
	{4,5,8,0,2},
	{4,3,0,6,8},
	{4,3,6,0,2},
	{4,7,6,2,8},
	{4,7,8,0,6},
	{1,3,0,2,6},
	{1,3,4,5,7},
	{1,5,2,0,8},
	{1,5,4,3,7},
	{7,3,6,0,8},
	{7,3,4,1,5},
	{7,5,8,2,6},
	{7,5,4,1,3}};
	for(int i = 0;i < cases;++i){
		if(board[data[i][0]] == turn && board[data[i][1]] == turn && board[data[i][2]] == 0 && board[data[i][3]] == 0 && board[data[i][4]] == 0){
			if(oneAway(oTurn)){
				if(getOneAway(oTurn) == data[i][2])
					return 1;
			}
			else
				return 1;
		}
	}
	return 0;
}
