#include <list>
#include <stdint.h>
#include <mutex>

class ConnectFour {
private:
	uint8_t Gameboard [6][7];
	// return 0 if no one wins, return 1 or 2 if someone wins, 
	// return -1 if fail to insect
	uint8_t Check_Win ( uint8_t x, uint8_t y );

public:
	// default constructor
	ConnectFour(); 

	// copy constructor
	ConnectFour(ConnectFour *game);
	~ConnectFour();

	// print the current game board
	void Print_Game_Board();
	
	int Make_a_Move ( uint8_t col );
	inline bool Is_Over() { return this->Possible_Choice.empty() == true; }
	inline int Get_Total_Move () { return this->Total_Move; }

	uint8_t Random_Possible_Choice ();
private:
	std::list<uint8_t> Possible_Choice;
	uint8_t Total_Move;
};


// Returns calculated next move (best calculated next move). Accruicy is restricted by
// current game status, pre defined number of playouts and time limits.
uint8_t AI_decision (ConnectFour *game, uint64_t playouts, uint8_t time_limit);

