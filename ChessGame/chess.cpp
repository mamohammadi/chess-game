#include "chess.h"

point chess::getPoint()
{
	int p;
	cin >> p;
	return point(p / 10, p % 10);
}

void chess::printPlayerName()
{
	if (player == BlackPlayer)
		cout << "BLACK PLAYER";
	else
		cout << "WHITE PLAYER";
}

void chess::changePlayer()
{
	if (player == BlackPlayer)
		player = WhitePlayer;
	else
		player = BlackPlayer;
}

bool chess::isInRange(point p)
{
	if (p.R >= 0 && p.R <= 7 && p.C >= 0 && p.R <= 7)
		return true;
	return false;
}

chess::chess()
{
}

chess::~chess()
{
}

void chess::start()
{
	player = WhitePlayer; // White player always moves first
	board gameBoard;
	gameBoard.set();
	while (true)
	{
		cout << endl;
		printPlayerName();
		cout << " enter move: ";

		point from = getPoint();
		char ch;
		cin >> ch >> ch;
		point to = getPoint();

		if (isInRange(from) && isInRange(to))
		{
			bool done = gameBoard.move(player, from, to);
			if (done)
			{
				// Check if finished
				if (gameBoard.gameFinished())
					return;

				// Change player
				changePlayer();
			}
			else
				cout << "Invalid move !!!!";
		}
		else
			cout << "Invalid points !!!!";
	}
}
