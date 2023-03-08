#include "chess.h"

using namespace std;

int main()
{
	chess game;
	while (true)
	{
		game.start();
		cout << "\nGame finished. Restart(Y or y)? ";
		char answer;
		cin >> answer;
		if (answer != 'Y' && answer != 'y')
			break;
	}

	return 0;
}