#include <iostream>
#include "Connect4.h"
int main () {
	std::cout << "program starts\n";


	ConnectFour game;
	game.Print_Game_Board();

	//fill the second col on the game board
	std:: cout << game.Make_a_Move (1) << std:: endl;
	game.Print_Game_Board();
	std:: cout << game.Make_a_Move (1) << std:: endl;
	game.Print_Game_Board();
	std:: cout << game.Make_a_Move (1) << std:: endl;
	game.Print_Game_Board();
	std:: cout << game.Make_a_Move (1) << std:: endl;
	game.Print_Game_Board();
	std:: cout << game.Make_a_Move (1) << std:: endl;
	game.Print_Game_Board();
	std:: cout << game.Make_a_Move (1) << std:: endl;
	game.Print_Game_Board();
	

	//this one should fail.
	std:: cout << game.Make_a_Move (1) << std:: endl;

	std::cout << "Total move: " << +game.Get_Total_Move() << std::endl;

	return 0;
}