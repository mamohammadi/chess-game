#include "board.h"

class chess
{
private:
	int player;

	point getPoint();
	void printPlayerName();
	void changePlayer();
	bool isInRange(point p);
public:
	chess();
	~chess();

	void start();
};

