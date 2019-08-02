#include "Connect4.h"
#include <iostream>
#include <algorithm> 
#include <thread>
#include <chrono>
#include <assert.h>
#include <mutex>
#include <ctime>    // For time()
#include <cstdlib>  // For srand() and rand()

bool SHOW_STATS = false; // release developer friendly prints

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
void AI_play (uint8_t first_move, float *stats, ConnectFour *game, uint64_t playouts, uint8_t *terminate, Thread_lock *instance ){
	
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
		std::cout << "== Child thread-" << std::this_thread::get_id() << " responsible for position "<< +first_move << " is terminated. # of playouts finished: "<< run_count-1 << std::endl;
	}
	instance->finished_count++;
	instance->finished_count_lock.unlock();
}

uint8_t AI_decision (ConnectFour *game, uint64_t playouts = 500, uint8_t time_limit = 3){
	std::cout << "Waiting for AI to make decision..." << std::endl;
	float stats[7] = {-1,-1,-1,-1,-1,-1,-1}; //random playout results
	uint8_t terminate = 0; // 'singal flag' used to force terminate threads
	Thread_lock instance; // keep track the number of finished threads, protected by single mutex
	assert (instance.finished_count == 0);

	//create 7 threads to random play with each move
	std::thread thread0( AI_play, 0, &(stats[0]), game, playouts, &terminate, &instance );
	std::thread thread1( AI_play, 1, &(stats[1]), game, playouts, &terminate, &instance );
	std::thread thread2( AI_play, 2, &(stats[2]), game, playouts, &terminate, &instance );
	std::thread thread3( AI_play, 3, &(stats[3]), game, playouts, &terminate, &instance );
	std::thread thread4( AI_play, 4, &(stats[4]), game, playouts, &terminate, &instance );
	std::thread thread5( AI_play, 5, &(stats[5]), game, playouts, &terminate, &instance );
	std::thread thread6( AI_play, 6, &(stats[6]), game, playouts, &terminate, &instance );

	// while still time, main thread waits child threads to finish
	auto start = std::chrono::steady_clock::now();
	while ( std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start).count() < time_limit ){
		//if all threads are finished, stop waiting.
		if (instance.finished_count >= 7){ 
			terminate = 1;
			if (SHOW_STATS){
				std::cout << "== Main thread: all threads finished before time limit (" << +time_limit << " sec).\n";
			}
			break;
		}
	}// used up time limit

	//send a signal to all child threads, force them to terminate.
	if (instance.finished_count != 7){
		terminate = 1; 
		if (SHOW_STATS){
			std::cout << "== Main thread: run over time limit (" << +time_limit << " sec). Threads are forced to terminate\n";
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
		std::cout << "== Calulated probability is: [ ";
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



// Thread runner, responsible for calculation of steps after the 'first move'
void AI_play_v2 (uint8_t first_move, float *stats, ConnectFour *game, uint64_t playouts, uint8_t *terminate, Thread_lock *instance ){
	
	uint64_t run_count = 1, win_count = 0, draw_count=0, lose_count = 0;
	bool check_condition = true;

	//terminate==1 -> force quit(main thread signal)  OR  run_count == playouts -> finish and quit (local)
	while (*terminate != 1 && run_count < playouts){ 
		run_count ++;
		
		//playouts-------
		ConnectFour local_game_cpy(game);
		int result = local_game_cpy.Make_a_Move(first_move);

		if (result == -1){ //firt move not legal -> thread not legal, terminated.
			// std::cout << "not legal\n";
			goto endWhile;
		}else if (result != 0){ // AI wins
			std::cout << "AI wins immidiately, don't MCTs: " << +first_move << std::endl;
			win_count += 2;

			//stop all other thread =======================
				instance->finished_count_lock.lock();
				if (SHOW_STATS){
					std::cout << "== Child thread-" << std::this_thread::get_id() << " responsible for position "<< +first_move << " Found the win move\n";
				}
				instance->finished_count = 7;
				assert(instance->finished_count<=7);
				instance->finished_count_lock.unlock();
			//stop all other thread =======================
			
			goto endWhile;
		}else if (check_condition){
			int arr[7] = {0, 0, 0, 0, 0, 0, 0};
			for (int human_move = 0; human_move < 7; human_move++){
				ConnectFour h_game(&local_game_cpy);
				int human_result = h_game.Make_a_Move(human_move);
				if (human_result == 1 || human_result == 2){ // human can win immidiately if AI make first_move -> AI shouldn't play there
					win_count = 0;
					std::cout << "human wins immidiately if AI plays at " << +first_move <<", don't MCTs" << std::endl;
					// stop current thread
					goto endWhile;
				}
				for (int ai_move = 0; ai_move < 7; ai_move++){
					ConnectFour ai_game(&h_game);
					int ai_result = ai_game.Make_a_Move(ai_move);
					if (ai_result == 1 || ai_result == 2){
						// local_game_cpy.Print_Game_Board();
						std::cout << "current ai test: " << +first_move << std::endl;
						std::cout << "human moves " << human_move << ", AI moves " << ai_move << " will win" << std::endl;
						arr[human_move] = 1;
						break;
					}else if (ai_result == -1){
						arr[human_move] = -1;
					}
				}
			}
			int numberOfWins = 0;
			
			for (int i = 0; i < 7; i++){
				if (arr[i]==1 || arr[i]==-1){
					numberOfWins += 1;
				}
			}
			
			if (numberOfWins == 7){ // AI can win if make first_move; no matter what human play next, AI has a position to win immidiately then
				win_count += 2;
				std::cout << "AI wins in 2 steps, don't MCTs: " << +first_move << std::endl;
				std::cout << "printing array" << std::endl;
				for (int i = 0; i < 7; i++)
					std::cout << arr[i] << " ";
				std::cout << std::endl;
				//stop all other thread =======================
				instance->finished_count_lock.lock();
				if (SHOW_STATS){
					std::cout << "== Child thread-" << std::this_thread::get_id() << " responsible for position "<< +first_move << " Found the win move\n";
				}
				instance->finished_count = 7;
				assert(instance->finished_count<=7);
				instance->finished_count_lock.unlock();
				//stop all other thread =======================

				goto endWhile;
			}
			check_condition = false;
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

	endWhile:
	//write the calculated result back to the corresponding location
	*stats = float (float(win_count)/float(run_count)); 

	//tell main thread that this thread finishes
	instance->finished_count_lock.lock();
	if (SHOW_STATS){
		std::cout << "== Child thread-" << std::this_thread::get_id() << " responsible for position "<< +first_move << " is terminated. # of playouts finished: "<< run_count << std::endl;
	}
	instance->finished_count++;
	instance->finished_count_lock.unlock();
}

uint8_t AI_decision_v2 (ConnectFour *game, uint64_t playouts = 500, uint8_t time_limit = 3){
	std::cout << "Waiting for AI to make decision..." << std::endl;
	float stats[7] = {-1,-1,-1,-1,-1,-1,-1}; //random playout results
	uint8_t terminate = 0; // 'singal flag' used to force terminate threads
	Thread_lock instance; // keep track the number of finished threads, protected by single mutex
	assert (instance.finished_count == 0);

	//create 7 threads to random play with each move
	std::thread thread0( AI_play_v2, 0, &(stats[0]), game, playouts, &terminate, &instance );
	std::thread thread1( AI_play_v2, 1, &(stats[1]), game, playouts, &terminate, &instance );
	std::thread thread2( AI_play_v2, 2, &(stats[2]), game, playouts, &terminate, &instance );
	std::thread thread3( AI_play_v2, 3, &(stats[3]), game, playouts, &terminate, &instance );
	std::thread thread4( AI_play_v2, 4, &(stats[4]), game, playouts, &terminate, &instance );
	std::thread thread5( AI_play_v2, 5, &(stats[5]), game, playouts, &terminate, &instance );
	std::thread thread6( AI_play_v2, 6, &(stats[6]), game, playouts, &terminate, &instance );

	// while still time, main thread waits child threads to finish
	auto start = std::chrono::steady_clock::now();
	while ( std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start).count() < time_limit ){
		//if all threads are finished, stop waiting.
		if (instance.finished_count >= 7){ 
			terminate = 1;
			if (SHOW_STATS){
				std::cout << "== Main thread: all threads finished before time limit (" << +time_limit << " sec).\n";
			}
			break;
		}
	}// used up time limit

	//send a signal to all child threads, force them to terminate.
	if (instance.finished_count != 7){
		terminate = 1; 
		if (SHOW_STATS){
			std::cout << "== Main thread: run over time limit (" << +time_limit << " sec). Threads are forced to terminate\n";
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
		std::cout << "== Calulated probability is: [ ";
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