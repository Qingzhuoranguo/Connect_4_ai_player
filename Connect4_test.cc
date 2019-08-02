#include <assert.h>
#include <iostream>
#include "Connect4.h"
#include <ctime>    
#include <cstdlib>  

#define PLAYOUTS 1000000 //max random playouts the AI will make.
#define TIME_LIMIT 2 //time limit for each decision calculation




void Play_Game ( uint8_t version );
// void Play_Game_auto (int* w, int* d, int* l);

int main () {
	Set_Developer_Mode (true);
	std::cout << "\n==================Program starts==================\n" << std::endl;
	std::srand((int) time(0));
	while (true){
		Play_Game(2);
		bool cont = false;
		std::string buff = "";
		std::string s1("y");
		std::string s2("Y");
		std::string s3("N");
		std::string s4("n");
		while (true){
			std::cout << "Do you want to play again? (Y/n) ";
			std::cin >> buff;
			// getline (std::cin, buff);
			if ( (buff.compare (s1)) == 0 || (buff.compare (s2)) == 0){
				cont = true;
				break;
			}else if((buff.compare (s3)) == 0 || (buff.compare (s4)) == 0){
				std::cout << "Goodbye!\n";
				break;
			}else {
				std::cout << "Invalid input detected\n";
			}
		}
		if (cont){
			continue;
		}else {
			break;
		}
	}
	std::cout << "\n==================Program ends==================\n" << std::endl;

	// int count[3] = {0,0,0};
	// Play_Game_auto(&(count[0]), &(count[1]), &(count[2]));
	// std::cout << "AI win count: " << count[0] << ", AI tie count: " << count[1] << ", AI lose count: " << count[2] << std::endl;
	// return 0;
}


void Play_Game (uint8_t version){
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
	std::cout << "Game starts:\n" << std::endl;
	
	ConnectFour game;
	char Human_Move;
	int result = 0;
	int AI_Move;

	if (Human_First){
		game.Print_Game_Board();
		std::cout << "Please make your first move: ";
		std::cin >> Human_Move;
		while ( game.Is_Valid(int(Human_Move)-48)==false ){
			std::cout << "Your input is invalid, please make another valid input.";
			std::cin >> Human_Move;
		}
		result = game.Make_a_Move (int(Human_Move)-48);
		assert (result != -1);
		game.Print_Game_Board();
		if (result != 0){ //human wins
			std::cout << "You win!\n";
			return;
		}
	}

	while ( !game.Is_Over() ){
		AI_Move = AI_decision(&game, PLAYOUTS, TIME_LIMIT, version);
		result = game.Make_a_Move( AI_Move );
		assert (result != -1);
		std::cout << "AI choose to place at: " << AI_Move << std::endl;
		game.Print_Game_Board();
		if (result != 0){ //AI wins
			std::cout << "You lose!\n";
			return;
		}

		std::cout << "Please make your move: ";
		std::cin >> Human_Move;
		while ( game.Is_Valid(int(Human_Move)-48) == false ){
			std::cout << "Your input is invalid, please make another valid input.";
			std::cin >> Human_Move;
		}
		result = game.Make_a_Move (int(Human_Move)-48);
		assert (result != -1);
		game.Print_Game_Board();
		if (result != 0){ //human wins
			std::cout << "You win!\n";
			return;
		}
	}
	std::cout << "Tied!\n";
}


// void Play_Game_auto (int* w, int* d, int* l){
	
// 	bool Human_First = false;
// 	std::cout << "Game starts:\n" << std::endl;
	
// 	ConnectFour game;
// 	int result = 0;
// 	int Human_Move;
// 	int AI_Move;

// 	if (Human_First){
// 		game.Print_Game_Board();
// 		Human_Move = AI_decision(&game, PLAYOUTS, TIME_LIMIT, 1);
// 		result = game.Make_a_Move (Human_Move);
// 		assert (result != -1);
// 		game.Print_Game_Board();
// 		std::cout << "AI1 choose to place at: " << Human_Move << std::endl;
// 	}

// 	while ( !game.Is_Over() ){
// 		AI_Move = AI_decision(&game, PLAYOUTS, TIME_LIMIT, 2);
// 		std::cout << "AI2 choose to place at: " << AI_Move << std::endl;
// 		result = game.Make_a_Move( AI_Move );
// 		assert (result != -1);
// 		game.Print_Game_Board();
// 		if (result != 0){ //AI wins
// 			std::cout << "AI2 win!\n";
// 			(*w)++;
// 			std::cout << *w++ << std::endl;
// 			return;
// 		}

// 		Human_Move = AI_decision(&game, PLAYOUTS, TIME_LIMIT, 1);
// 		std::cout << "AI1 choose to place at: " << Human_Move << std::endl;
// 		result = game.Make_a_Move (Human_Move);
// 		assert (result != -1);
// 		game.Print_Game_Board();
// 		if (result != 0){ //human wins
// 			std::cout << "AI1 win!\n";
// 			*l++;
// 			std::cout << *l++ << std::endl;
// 			return;
// 		}
// 	}
// 	std::cout << "Tied!\n";
// 	*d++;
// }