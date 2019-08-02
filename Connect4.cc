#include "Connect4.h"
#include <iostream>
#include <algorithm> 
#include <thread>
#include <chrono>
#include <assert.h>
#include <mutex>
#include <ctime>    // For time()
#include <cstdlib>  // For srand() and rand()

bool SHOW_STATS = false;

void Set_Developer_Mode( bool set ){
	if (set){
		std::cout << "==Developer== Developer Mode: on\n";
		SHOW_STATS = true;
	}else {
		std::cout << "==Developer== Developer Mode: off\n";
	}
}

// keep track of how many threads are finished, protected by single mutex
struct Thread_lock{
	std::mutex finished_count_lock;
	uint8_t finished_count;
	Thread_lock(){ finished_count = 0;};
	~Thread_lock(){};
};


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

ConnectFour::ConnectFour(ConnectFour *game){
	//initialize the gameboard to be 0's -> empty
	for (int i = 0; i < 6; i ++){
		for (int j = 0; j < 7; j++){
			this->Gameboard[i][j] = game->Gameboard[i][j];
		}
	}
	this->Total_Move = game->Total_Move;

	//initialize all the legal moves.
	for ( auto v : game->Possible_Choice ){
		this->Possible_Choice.push_back(v);
	}
}

ConnectFour::~ConnectFour(){
	return;
}

void ConnectFour::Print_Game_Board_simple(){
	std::cout << "The current game board is: \n";
	for (int i = 5; i >-1; i --){
		std::cout << "| ";
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
		std::cout << "| ";
		std::cout << std::endl;
	}
	std::cout << "| 0 1 2 3 4 5 6 |" << std::endl;
}

void ConnectFour::Print_Game_Board(){
	std::cout << "The current game board is: \n";
	for (int i = 5; i >-1; i --){
		std::cout<<"   +---+---+---+---+---+---+---+"<<std::endl;
		for (int j = 0; j < 7; j ++){
			if (j==0){
				std::cout << " " << char(97+i) << " |";
			}
			if (this->Gameboard[i][j]==0){
				std::cout << "  ";
			}else if (this->Gameboard[i][j]==1){
				std::cout << " X";
			}else{
				std::cout << " O";
			}
			std::cout <<" |";
		}
		std::cout << std::endl;
	}
	std::cout<<"   +---+---+---+---+---+---+---+"<<std::endl;
	std::cout<<"     0   1   2   3   4   5   6  "<<std::endl <<std::endl;
}

int ConnectFour::Make_a_Move ( uint8_t col ){
	std::list<uint8_t>::iterator it;
	it = std::find (this->Possible_Choice.begin(), this->Possible_Choice.end(), col);
	
	//movement col is not in the legal movement set
	if (it == this->Possible_Choice.end()){
		return -1; 
	}
	
	uint8_t player = 0;
	if (this->Total_Move %2 == 0){
		player = 1;
	}else {
		player = 2;
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
	//if someone wins, no more legal next move.
	if (result != 0){
		this->Possible_Choice.clear();
	}
	return result;
}

bool ConnectFour::Is_Valid ( uint8_t col ){
	for (auto v : this->Possible_Choice){
		if (v == col){
			return true;
		}
	}
	return false;
}

uint8_t ConnectFour::Check_Win ( uint8_t x, uint8_t y ){
	if (x>2)
		if (this->Gameboard[x][y]==this->Gameboard[x-1][y] && 
			this->Gameboard[x][y]==this->Gameboard[x-2][y] && 
			this->Gameboard[x][y]==this->Gameboard[x-3][y])
			
			return this->Gameboard[x][y];
	// horizontal
	for (int c = 0; c < std::min(4,y+1); c++)
		if (y-c<4 && this->Gameboard[x][0+c]==this->Gameboard[x][1+c] && 
			this->Gameboard[x][0+c]==this->Gameboard[x][2+c] && 
			this->Gameboard[x][0+c]==this->Gameboard[x][3+c])

			return this->Gameboard[x][y];
	// diagonal up right
	int r = x+3;
	int c = y-3;
	// std::cout << "x=" << x << "y=" << y << std::endl;
	for (uint8_t k = 0; k < 4; k++){
		if (r<6 && r-3>-1 && c>-1 && c+3<7){
			if (this->Gameboard[r][c]==this->Gameboard[r-1][c+1] &&
				this->Gameboard[r][c]==this->Gameboard[r-2][c+2] &&
				this->Gameboard[r][c]==this->Gameboard[r-3][c+3])

				return this->Gameboard[x][y];
		}
		r--;
		c++;
	}

	// diagonal up left
	r = x+3;
	c = y+3;
	//std::cout << "x=" << +x << "y=" << +y << std::endl;
	// std::cout << "r=" << r << "c=" << c << std::endl;
	for (uint8_t k = 0; k < 4; k++){
		// std::cout << "diagonal up left" << std::endl;
		//std::cout << "r=" << r << "c=" << c << std::endl;
		if (r<6 && r-3>-1 && c<7 && c-3>-1){
			//std::cout << "satisfied" << std::endl;
			if (this->Gameboard[r][c]==this->Gameboard[r-1][c-1] &&
				this->Gameboard[r][c]==this->Gameboard[r-2][c-2] &&
				this->Gameboard[r][c]==this->Gameboard[r-3][c-3])

				return this->Gameboard[x][y];
		}
		r--;
		c--;
	}
	return 0;
}

uint8_t ConnectFour::Random_Possible_Choice (){
	if (this->Possible_Choice.size()==0){
		std::cout << "error\n";
	}
	uint8_t index = (rand()%this->Possible_Choice.size());
	uint8_t i = 0;
	for ( auto v : this->Possible_Choice ){
		if (i == index){
			return v;
		}
		i ++;
	}	
}

// Thread runner, responsible for calculation of steps after the 'first move'
void AI_play_v1 (uint8_t first_move, float *stats, ConnectFour *game, uint64_t playouts, uint8_t *terminate, Thread_lock *instance ){
	
	uint64_t run_count = 1, win_count = 0, draw_count=0, lose_count = 0;

	//terminate==1 -> force quit(main thread signal)  OR  run_count == playouts -> finish and quit (local)
	while (*terminate != 1 && run_count < playouts){ 
		run_count ++;
		
		//playouts-------
		ConnectFour local_game_cpy(game);
		int result = local_game_cpy.Make_a_Move(first_move);

		if (result == -1){ //firt move not legal -> thread not legal, terminated.
			// std::cout << "not legal\n";
			break;
		}else if (result != 0){ // AI wins
			win_count += 1;
			break;
		}else { //draw, invoke random_fill to play
			bool tie = true;
			while ( local_game_cpy.Is_Over()==false ){
				uint8_t move = local_game_cpy.Random_Possible_Choice ();
				assert ( move < 7 && move >=0);


				result = local_game_cpy.Make_a_Move (move);

				assert (result != -1);
				if (result != 0){ //human wins
					lose_count ++;
					tie = false;
					break;
				}else{ //after human move, tied
					if (local_game_cpy.Is_Over() == true){
						break;
					}
					move = local_game_cpy.Random_Possible_Choice ();
					assert ( move < 7 && move >=0);
					
					result = local_game_cpy.Make_a_Move (move); // AI makes a move

					if (result != 0){ // AI wins
						win_count ++;
						tie = false;
						break;
					}else{ //tie
						continue;
					}
				}
				local_game_cpy.Print_Game_Board();
			}
			if (local_game_cpy.Is_Over() && tie){
				draw_count ++;
			}

		}
		//---------------
	}
	//write the calculated result back to the corresponding location
	*stats = float (float(win_count)/float(run_count)); 

	//tell main thread that this thread finishes
	instance->finished_count_lock.lock();
	if (SHOW_STATS){
		std::cout << "==Developer== Child thread-" << std::this_thread::get_id() << " responsible for position "<< +first_move << " is terminated. # of playouts finished: "<< run_count-1 << std::endl;
	}
	instance->finished_count++;
	instance->finished_count_lock.unlock();
}

// Thread runner, responsible for calculation of steps after the 'first move'
void AI_play_v2 (uint8_t first_move, float *stats, ConnectFour *game, uint64_t playouts, uint8_t *terminate, Thread_lock *instance ){
	
	uint64_t run_count = 1, win_count = 0, draw_count=0, lose_count = 0;

	bool checked = false;
	//terminate==1 -> force quit(main thread signal)  OR  run_count == playouts -> finish and quit (local)
	while (*terminate != 1 && run_count < playouts){ 
		run_count ++;
		
		//playouts-------
		ConnectFour local_game_cpy(game);
		int result = local_game_cpy.Make_a_Move(first_move);

		if (result == -1){ //firt move not legal -> thread not legal, terminated.
			goto endWhile;
		}else if (result != 0){ // AI wins
			assert (result == 1 || result == 2);
			*stats = 2;
			//stop all other thread =======================
			instance->finished_count_lock.lock();
			if (SHOW_STATS){
				std::cout << "==Developer== Child thread-" << std::this_thread::get_id() << " responsible for position "<< +first_move << " Found the win move\n";
			}
			instance->finished_count = 7;
			assert(instance->finished_count<=7);
			instance->finished_count_lock.unlock();
			//stop all other thread =======================
			goto endWhile;
		}else { //draw

			if (!checked){
				//check if first_move is Good Move======================================================
				bool ai_win_all = true;

				for (int human_move = 0; human_move < 7; human_move++){
					ConnectFour h_game(&local_game_cpy);
					int human_result = h_game.Make_a_Move(human_move);
					if (human_result == -1){
						continue;
					}
					//if human can win first, no need to consider Good Move
					if (human_result == 1 || human_result == 2){ // human can win immidiately if AI make first_move -> AI shouldn't play there
						*stats = -2;
						if (SHOW_STATS){
							std::cout << "==Developer== Child thread-" << std::this_thread::get_id() << " responsible for position "<< +first_move << " Found the lose move\n";
						}
						goto endWhile;
					}
					bool ai_win = false;
					for (int ai_move = 0; ai_move < 7; ai_move++){
						ConnectFour ai_game(&h_game);
						int ai_result = ai_game.Make_a_Move(ai_move);
						if (ai_result == 1 || ai_result == 2){
							ai_win = true;
							break;
						}
					}
					if (!ai_win){
						ai_win_all = false;
					}
				}
				
				if (ai_win_all){ // AI can win if make first_move; no matter what human play next, AI has a position to win immidiately then
					*stats = 2;

					//stop all other thread =======================
					instance->finished_count_lock.lock();
					if (SHOW_STATS){
						std::cout << "==Developer== Child thread-" << std::this_thread::get_id() << " responsible for position "<< +first_move << " Found the good move\n";
					}
					instance->finished_count = 7;
					assert(instance->finished_count<=7);
					instance->finished_count_lock.unlock();
					//stop all other thread =======================

					goto endWhile;
				}


				//check if human has a Good Move===============================================
				bool human_is_good_move = false;

				for (uint8_t human_move = 0; human_move < 7; human_move++){
					ConnectFour h_game(&local_game_cpy);
					int human_result = h_game.Make_a_Move(human_move);
					if (human_result == -1){ 
						continue;
					}

					//checked previously cannot be 1 or 2 so draw:

					bool human_win_all = true;
					for (uint8_t ai_it_move = 0; ai_it_move < 7; ai_it_move ++){ // for each ai possible moves
						ConnectFour hh_game(&h_game);
						int ai_it_result = hh_game.Make_a_Move(ai_it_move);

						if (ai_it_move == -1 || ai_it_result == 1 || ai_it_result == 2){ //if ai wins, then human_move is not a good move
							continue;
						}


						//draw, check if human can win in anyway
						bool human_win = false;
						for (uint8_t human_try = 0; human_try < 7; human_try++){
							ConnectFour hhh_game(hh_game);
							int hhh_game_result = hhh_game.Make_a_Move(human_try);

							if (hhh_game_result == -1){
								continue;
							}
							if (hhh_game_result == 1 || hhh_game_result == 2){ //wins
								human_win = true;
								break;
							}
						}
						if (!human_win){
							human_win_all = false;
						}
					}
					if (human_win_all){ //is Good Move
						*stats = -2;
						goto endWhile;
					}

				}
				checked = true;
			}// if (checked)

			//random playouts:
			bool tie = true;
			while ( local_game_cpy.Is_Over()==false ){
				uint8_t move = local_game_cpy.Random_Possible_Choice ();
				assert ( move < 7 && move >=0);

				result = local_game_cpy.Make_a_Move (move);

				assert (result != -1);
				if (result != 0){ //human wins
					lose_count ++;
					tie = false;
					break;
				}else{ //after human move, tied
					if (local_game_cpy.Is_Over() == true){
						break;
					}
					move = local_game_cpy.Random_Possible_Choice ();
					assert ( move < 7 && move >=0);
					
					result = local_game_cpy.Make_a_Move (move); // AI makes a move

					if (result != 0){ // AI wins
						win_count ++;
						tie = false;
						break;
					}else{ //tie
						continue;
					}
				}
				local_game_cpy.Print_Game_Board();
			}
			if (local_game_cpy.Is_Over() && tie){
				draw_count ++;
			}

		}
		//---------------
	}

	//write the calculated result back to the corresponding location
	*stats = float (float(win_count)/float(run_count)); 

	endWhile:
	//tell main thread that this thread finishes
	instance->finished_count_lock.lock();
	if (SHOW_STATS){
		std::cout << "==Developer== Child thread-" << std::this_thread::get_id() << " responsible for position "<< +first_move << " is terminated. # of playouts finished: "<< run_count << std::endl;
	}
	instance->finished_count++;
	instance->finished_count_lock.unlock();
}

uint8_t AI_decision (ConnectFour *game, uint64_t playouts = 500, uint8_t time_limit = 3, uint8_t version =1){
	std::cout << "Waiting for AI to make decision..." << std::endl;
	float stats[7] = {-1,-1,-1,-1,-1,-1,-1}; //random playout results
	uint8_t terminate = 0; // 'singal flag' used to force terminate threads
	Thread_lock instance; // keep track the number of finished threads, protected by single mutex
	assert (instance.finished_count == 0);

	void (*Runner)(uint8_t, float *, ConnectFour *, uint64_t, uint8_t *, Thread_lock *);
	if (version == 1){
		Runner = &AI_play_v1;
	}else if ( version == 2){
		Runner = &AI_play_v2;
	}else {
		std::cout << "AI_decision: version number not recognized, using default version: 1. \n";
		Runner = &AI_play_v1;
	}
	


	//create 7 threads to random play with each move
	std::thread thread0( Runner, 0, &(stats[0]), game, playouts, &terminate, &instance );
	std::thread thread1( Runner, 1, &(stats[1]), game, playouts, &terminate, &instance );
	std::thread thread2( Runner, 2, &(stats[2]), game, playouts, &terminate, &instance );
	std::thread thread3( Runner, 3, &(stats[3]), game, playouts, &terminate, &instance );
	std::thread thread4( Runner, 4, &(stats[4]), game, playouts, &terminate, &instance );
	std::thread thread5( Runner, 5, &(stats[5]), game, playouts, &terminate, &instance );
	std::thread thread6( Runner, 6, &(stats[6]), game, playouts, &terminate, &instance );

	// while still time, main thread waits child threads to finish
	auto start = std::chrono::steady_clock::now();
	while ( std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start).count() < time_limit ){
		//if all threads are finished, stop waiting.
		if (instance.finished_count >= 7){ 
			terminate = 1;
			if (SHOW_STATS){
				std::cout << "==Developer== Main thread: all threads finished before time limit (" << +time_limit << " sec).\n";
			}
			break;
		}
	}// used up time limit

	//send a signal to all child threads, force them to terminate.
	if (instance.finished_count != 7){
		terminate = 1; 
		if (SHOW_STATS){
			std::cout << "==Developer== Main thread: run over time limit (" << +time_limit << " sec). Threads are forced to terminate\n";
		}
	}

	thread0.join();
	thread1.join();
	thread2.join();
	thread3.join();
	thread4.join();
	thread5.join();
	thread6.join();

	if (SHOW_STATS){
		std::cout << "==Developer== Calulated probability is: [ ";
		for (uint8_t i = 0; i < 7; i ++){
			std::cout << stats[i] << " ";
		}
		std::cout << "]"<<std::endl;
	}

	//find the max probability in stats and return the index (the move)
	float max = stats[0];
	uint8_t index = 0;
	for (uint8_t i = 0; i < 7; i ++){
		if ( stats[i]> max ){
			max = stats[i];
			index = i;
		}
	}
	return index;
}


