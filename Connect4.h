#include <list>
#include <stdint.h>


class ConnectFour {
private:
	uint8_t Gameboard [6][7];
	uint8_t Check_Win ( uint8_t x, uint8_t y );

public:
	ConnectFour();
	~ConnectFour();

	void Print_Game_Board();
	int Make_a_Move ( uint8_t col );
	inline int Get_Total_Move () { return this->Total_Move; }
private:
	std::list<uint8_t> Possible_Choice;
	uint8_t Total_Move;

};


class AI{
private:
	uint8_t random_fill(ConnectFour game);
	uint8_t stats[7] = -1;

public:
	AI(ConnectFour game);
	~AI();

	uint8_t AImove(ConnectFour game);

}
