#include <assert.h>
#include <iostream>
#include "Connect4.h"
#include <ctime>    // For time()
#include <cstdlib>  // For srand() and rand()

#define PLAYOUTS 100000 //max random playouts the AI will make.
#define TIME_LIMIT 2 //max time limit for each decision calculation


int main () {
	std::cout << "\nProgram starts\n";
	std::srand((int) time(0));
	ConnectFour game;

	//fill the second col on the game board
	std::cout << game.Make_a_Move (1) << std::endl;
	std::cout << game.Make_a_Move (1) << std::endl;

	std::cout << game.Make_a_Move (1) << std::endl;
	std::cout << game.Make_a_Move (1) << std::endl;
	
	std::cout << game.Make_a_Move (1) << std::endl;
	std::cout << game.Make_a_Move (1) << std::endl;
	
	std::cout << game.Make_a_Move (1) << std::endl;
	
	game.Print_Game_Board();
	std::cout << "Done\n";

	ConnectFour game_cpy (&game);
	game_cpy.Print_Game_Board();
	std::cout << game_cpy.Make_a_Move(1)<< std::endl;

	//uint8_t move = AI_decision (&game, PLAYOUTS, TIME_LIMIT);
	//std::cout << move << std::endl;

	return 0;
}


