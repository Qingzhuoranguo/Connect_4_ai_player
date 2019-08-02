// Thread runner, responsible for calculation of steps after the 'first move'
void AI_play (uint8_t first_move, float *stats, ConnectFour *game, uint64_t playouts, uint8_t *terminate, Thread_lock *instance ){
	
	uint64_t run_count = 0, win_count = 0, draw_count=0, lose_count = 0;
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
				ConnectFour h_game(*local_game_cpy);
				int human_result = h_game.Make_a_Move(human_move);
				if (human_result == 1 || human_result == 2){ // human can win immidiately if AI make first_move -> AI shouldn't play there
					win_count = 0;
					// stop current thread
					goto endWhile;
				}
				for (int ai_move = 0; ai_move < 7; ai_move++){
					ConnectFour ai_game(*h_game);
					int ai_result = ai_game.Make_a_Move(ai_move);
					if (ai_result == 1 || ai_result == 2){
						arr[human_move] = 1;
						break;
					}else if (ai_result == -1){
						arr[human_move] = -1;
					}
				}
			}
			int numberOfWins = 0;
			for (int i = 0; i < 7; i++){
				if (arr[i]==1 || arr[i]==0){
					numberOfWins += 1;
				}
			}
			if (numberOfWins == 7){ // AI can win if make first_move; no matter what human play next, AI has a position to win immidiately then
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