#include <assert.h>
#include <iostream>
#include "Connect4.h"
#include <ctime>    // For time()
#include <cstdlib>  // For srand() and rand()

#define PLAYOUTS 100000000 //max random playouts the AI will make.
#define TIME_LIMIT 5 //time limit for each decision calculation

void Play_Game ();

int main () {
	std::cout << "\n==================Program starts==================\n";
	std::srand((int) time(0));
	Play_Game();
	
	return 0;
}

void Play_Game (){
	std::string buff;
	std::string s1("y");
	std::string s2("Y");
	std::string s3("N");
	std::string s4("n");
	std::cout << "Do you want to go first? (Y/n)";
	bool Human_First = false;
	while (true){
		getline (std::cin, buff);
		if ( (buff.compare (s1)) == 0 || (buff.compare (s2)) == 0){
			Human_First = true;
			break;
		}else if((buff.compare (s3)) == 0 || (buff.compare (s4)) == 0){
			break;
		}else {
			std::cout << "Invalid input detected. Please enter again: (Y/n)";
		}
	}
	std::cout << "Game starts:\n";
	
	ConnectFour game;
	int Human_Move = 0;
	int result = 0;

	if (Human_First){
		std::cout << "Please make your first move: ";
		std::cin >> Human_Move;
		while ( game.Is_Valid(Human_Move)==false ){
			std::cout << "Your input is invalid, please make another valid input.";
			std::cin >> Human_Move;
		}
		result = game.Make_a_Move (Human_Move);
		assert (result != -1);
		game.Print_Game_Board();
		if (result != 0){ //human wins
			std::cout << "You win!\n";
			return;
		}
	}
	while ( !game.Is_Over() ){
		
		result = game.Make_a_Move( AI_decision (&game, PLAYOUTS, TIME_LIMIT) );
		assert (result != -1);
		game.Print_Game_Board();
		if (result != 0){ //AI wins
			std::cout << "You lose!\n";
			return;
		}

		std::cout << "Please make your first move: ";
		std::cin >> Human_Move;
		while ( game.Is_Valid(Human_Move) ==false ){
			std::cout << "Your input is invalid, please make another valid input.";
			std::cin >> Human_Move;
		}
		result = game.Make_a_Move (Human_Move);
		assert (result != -1);
		game.Print_Game_Board();
		if (result != 0){ //human wins
			std::cout << "You win!\n";
			return;
		}
	}
	std::cout << "Tied!\n";

	
}