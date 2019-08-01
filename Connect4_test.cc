#include <assert.h>
#include <iostream>
#include "Connect4.h"
#include <ctime>    // For time()
#include <cstdlib>  // For srand() and rand()

#define PLAYOUTS 100000000 //max random playouts the AI will make.
#define TIME_LIMIT 5 //max time limit for each decision calculation


int main () {
	std::cout << "\nProgram starts\n";
	std::srand((int) time(0));
	ConnectFour game;
	int i;

	//fill the second col on the game board
	while (game.Is_Over()==false){
		game.Make_a_Move(AI_decision (&game, PLAYOUTS, TIME_LIMIT));
		game.Print_Game_Board();
		if (game.Is_Over()==true){
			std::cout << "AI wins" << std::endl;
			break;
		}
		std::cout << "Please input a position: ";
		std::cin >> i;
		game.Make_a_Move(i);
		game.Print_Game_Board();
		if (game.Is_Over()==true){
			std::cout << "human wins" << std::endl;
			break;
		}
	}
	
	game.Print_Game_Board();
	std::cout << "Done\n";


	return 0;
}


