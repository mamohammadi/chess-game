#include <iostream>
#include "point.h"

using namespace std;

static const int BlackPlayer = 0;
static const int WhitePlayer = 1;

class board
{
private:
	char state[8][8];
	bool blackChecked;
	bool whiteChecked;
	bool blackCheckmate;
	bool whiteCheckmate;
	bool blackStalemate;
	bool whiteStalemate;

	void print();
	int getOpponent(int player);
	int getItemOwner(char value);
	point getItemPoint(char value);
	bool canMovePawn(int player, point from, point to);
	bool canMoveRook(int player, point from, point to);
	bool canMoveHorse(int player, point from, point to);
	bool canMoveElephant(int player, point from, point to);
	bool canMoveQueen(int player, point from, point to);
	bool canMoveKing(int player, point from, point to);
	bool canMove(int player, point from, point to, bool updateCheckStatus, bool updateCheckmate);
	bool isChecked(int player, point kingPoint);
	void updateCheckStatus();
	bool isCheckmate(int player, bool checked, char king);
	bool isStalemate(int player, char king);
	void promote(char fromValue, point to);
public:
	board();
	~board();

	void set();
	bool move(int player, point from, point to);
	bool gameFinished();
};