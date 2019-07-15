#include "Connect4.h"
#include <iostream>
#include<algorithm> 

ConnectFour::ConnectFour(){
	//initialize the gameboard to be 0's -> empty
	for (int i = 0; i < 6; i ++){
		for (int j = 0; j < 7; j++){
			this->Gameboard[i][j] = 0;
		}
	}
	this->Total_Move = 0;

	//initialize all the legal moves.
	for (int i = 0; i < 7; i ++){
		this->Possible_Choice.push_back(i);
	}


}

ConnectFour::~ConnectFour(){
	return;
}

void ConnectFour::Print_Game_Board(){
	std::cout << std::endl << "The current game board is: \n";
	for (int i = 5; i >-1; i --){
		for (int j = 0; j < 7; j ++){
			if (this->Gameboard[i][j] == 0){
				std::cout << "* ";
			}else if (this->Gameboard[i][j] == 1){
				std::cout << "X ";
			}else if (this->Gameboard[i][j] == 2){
				std::cout << "O ";
			}else {
				std::cout << std::endl << "Error!" << std::endl;
				return;
			}
			
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	return;
}

int ConnectFour::Make_a_Move ( uint8_t col ){
	uint8_t player = 0;
	if (this->Total_Move %2 == 0){
		player = 1;
	}else {
		player = 2;
	}

	std::list<uint8_t>::iterator it;
	it = std::find (this->Possible_Choice.begin(), this->Possible_Choice.end(), col);
	
	//movement col is not in the legal movement set
	if (it == this->Possible_Choice.end()){
		return -1; 
	}

	//find a unoccupied spot (bottom up) to place.
	uint8_t pos = 0;
	for ( int i = 0; i < 6; i ++){
		if ( this->Gameboard[i][col] != 0 ){
			continue;
		}
		pos = i;
		break;
	}//should always find a legal spot, col with no legal spot should not be in Possible_Choice.

	//take the position 
	this->Gameboard[pos][col] = player;
	this->Total_Move ++;
	if (pos == 5){ //if the last spot in this col
		this->Possible_Choice.remove(col);
	}

	int result = Check_Win ( pos, col );
	std::cout << "result is: " << result << std::endl;
	//if someone wins, no more legal next move.
	if (result != 0){
		this->Possible_Choice.clear();
	}
	return result;
}

uint8_t ConnectFour::Check_Win ( uint8_t x, uint8_t y ){
	uint8_t player = this->Gameboard[x][y];
	uint8_t player_count = 1;

	//horizontal:
	//1) to the left:
	int8_t horizontal_iterater = y;
	while ( horizontal_iterater-1 >= 0 && horizontal_iterater-1 > y-4 ){
		horizontal_iterater --;
		if ( this->Gameboard [x][horizontal_iterater] == player ){
			player_count ++;
		}
		break;
	}
	if (player_count >=4){
		return this->Gameboard[x][y];
	}
	//2) to the right:
	horizontal_iterater = y;
	while ( horizontal_iterater+1 <= 6 && horizontal_iterater+1 < y+4 ){
		horizontal_iterater ++;
		if ( this->Gameboard [x][horizontal_iterater] == player ){
			player_count ++;
		}
		break;
	}
	if (player_count >=4){
		return this->Gameboard[x][y];
	}

	return 0;
}