#include <iostream>
#include "Connect4.h"
int main () {
	std::cout << "program starts\n";


	ConnectFour game;

	//fill the second col on the game board
	std:: cout << game.Make_a_Move (3) << std:: endl;
	std:: cout << game.Make_a_Move (6) << std:: endl;

	std:: cout << game.Make_a_Move (2) << std:: endl;
	std:: cout << game.Make_a_Move (6) << std:: endl;
	
	std:: cout << game.Make_a_Move (1) << std:: endl;
	std:: cout << game.Make_a_Move (6) << std:: endl;
	
	std:: cout << game.Make_a_Move (0) << std:: endl;
	
	game.Print_Game_Board();
	std::cout << "Done\n";

	std:: cout << game.Make_a_Move (3) << std:: endl;





	return 0;
}