#include "board.h"

int board::getItemOwner(char value)
{
	if (value == 'r' || value == 'h' || value == 'e' || value == 'q' || value == 'k' || value == 'p')
		return BlackPlayer; // Black Player
	if (value == 'R' || value == 'H' || value == 'E' || value == 'Q' || value == 'K' || value == 'P')
		return WhitePlayer; // White Player
	return -1; // Invalid Player
}

point board::getItemPoint(char value)
{
	for (int r = 0; r < 8; r++)
	{
		for (int c = 0; c < 8; c++)
		{
			if (state[r][c] == value)
				return point(r, c);
		}
	}
}

board::board()
{

}

board::~board()
{
}

void board::set()
{
	blackChecked = false;
	whiteChecked = false;
	blackCheckmate = false;
	whiteCheckmate = false;
	blackStalemate = false;
	whiteStalemate = false;

	char initState[8][8] = { 'r','h','e','q','k','e','h','r','p','p','p','p','p','p','p','p',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','P','P','P','P','P','P','P','P','R','H','E','Q','K','E','H','R' };
	for (int r = 0; r < 8; r++)
	{
		for (int c = 0; c < 8; c++)
			state[r][c] = initState[r][c];
	}
	print();
}

bool board::move(int player, point from, point to)
{
	char fromValue = state[from.R][from.C];
	char toValue = state[to.R][to.C];

	// Check Rules
	if (!canMove(player, from, to, false, false))
		return false;

	// Apply move
	state[to.R][to.C] = fromValue;
	state[from.R][from.C] = ' ';

	// Apply promotions
	promote(fromValue, to);

	// Update Check Status
	updateCheckStatus();

	if ((player == BlackPlayer && blackChecked) || (player == WhitePlayer && whiteChecked))
	{
		cout << "You are checked or will be checked. ";

		// Revert move
		state[from.R][from.C] = fromValue;
		state[to.R][to.C] = toValue;

		// Update Check Status
		updateCheckStatus();
		return false;
	}

	// Check Checkmate
	if (isCheckmate(BlackPlayer, blackChecked, 'k'))
		blackCheckmate = true;
	if (isCheckmate(WhitePlayer, whiteChecked, 'K'))
		whiteCheckmate = true;

	// Check Stalemate
	if (isStalemate(BlackPlayer, 'k'))
		blackStalemate = true;
	if (isStalemate(WhitePlayer, 'K'))
		whiteStalemate = true;

	print();
	return true;
}

bool board::canMovePawn(int player, point from, point to)
{
	int opponent = getOpponent(player);
	char fromValue = state[from.R][from.C];
	char toValue = state[to.R][to.C];

	if (player == BlackPlayer)
	{
		if (from.R == 1) // First move
		{
			if (getItemOwner(toValue) == -1 && (to.R == 2 || to.R == 3) && to.C == from.C) // Normal move
			{
				if (to.R == 3) // Two Step
				{
					if (getItemOwner(state[2][from.C]) != -1)
						return false;
					return true;
				}
				return true;
			}
			if (getItemOwner(toValue) == opponent && (to.R == from.R + 1) && (to.C == from.C + 1 || to.C == from.C - 1)) // Hit move
				return true;
			return false;
		}
		else // Other moves
		{
			if (getItemOwner(toValue) == -1 && (to.R == from.R + 1) && to.C == from.C) // Normal move
				return true;
			if (getItemOwner(toValue) == opponent && (to.R == from.R + 1) && (to.C == from.C + 1 || to.C == from.C - 1)) // Hit move
				return true;
			return false;
		}
	}
	else
	{
		if (from.R == 6) // First move
		{
			if (getItemOwner(toValue) == -1 && (to.R == 5 || to.R == 4) && to.C == from.C) // Normal move
			{
				if (to.R == 4) // Two Step
				{
					if (getItemOwner(state[5][from.C]) != -1)
						return false;
					return true;
				}
				return true;
			}
			if (getItemOwner(toValue) == opponent && (to.R == from.R - 1) && (to.C == from.C + 1 || to.C == from.C - 1)) // Hit move
				return true;
		}
		else // Other moves
		{
			if (getItemOwner(toValue) == -1 && (to.R == from.R - 1) && to.C == from.C) // Normal move
				return true;
			if (getItemOwner(toValue) == opponent && (to.R == from.R - 1) && (to.C == from.C + 1 || to.C == from.C - 1)) // Hit move
				return true;
			return false;
		}
	}
	return false;
}

bool board::canMoveRook(int player, point from, point to)
{
	if (to.R == from.R || to.C == from.C)
	{
		if (to.R == from.R)
		{
			int cStep;
			if (to.C - from.C < 0)
				cStep = -1;
			else
				cStep = 1;
			for (int c = from.C + cStep; c != to.C; c += cStep)
			{
				if (getItemOwner(state[from.R][c]) != -1)
					return false;
			}
			return true;
		}
		else
		{
			int rStep;
			if (to.R - from.R < 0)
				rStep = -1;
			else
				rStep = 1;
			for (int r = from.R + rStep; r != to.R; r += rStep)
			{
				if (getItemOwner(state[r][from.C]) != -1)
					return false;
			}
			return true;
		}
	}
	return false;
}

bool board::canMoveHorse(int player, point from, point to)
{
	if (to.R == from.R + 1 && (to.C == from.C + 2 || to.C == from.C - 2))
		return true;
	if (to.R == from.R + 2 && (to.C == from.C + 1 || to.C == from.C - 1))
		return true;
	if (to.R == from.R - 1 && (to.C == from.C + 2 || to.C == from.C - 2))
		return true;
	if (to.R == from.R - 2 && (to.C == from.C + 1 || to.C == from.C - 1))
		return true;
	return false;
}

bool board::canMoveElephant(int player, point from, point to)
{
	int rDist = to.R - from.R;
	int cDist = to.C - from.C;
	if (rDist == cDist || rDist == -cDist)
	{
		int rStep;
		int cStep;

		if (rDist < 0)
			rStep = -1;
		else
			rStep = 1;

		if (cDist < 0)
			cStep = -1;
		else
			cStep = 1;

		int c = from.C + cStep;
		for (int r = from.R + rStep; r != to.R; r += rStep)
		{
			if (getItemOwner(state[r][c]) != -1)
				return false;
			c += cStep;
		}
		return true;
	}
	return false;
}

bool board::canMoveQueen(int player, point from, point to)
{
	if (to.R == from.R || to.C == from.C)
		return canMoveRook(player, from, to);
	if ((to.R - from.R) == (to.C - from.C) || (to.R - from.R) == -(to.C - from.C))
		return canMoveElephant(player, from, to);
	return false;
}

bool board::canMoveKing(int player, point from, point to)
{
	int rDist = to.R - from.R;
	int cDist = to.C - from.C;
	if ((rDist == 1 || rDist == -1 || rDist == 0) && (cDist == 1 || cDist == -1 || cDist == 0))
		return true;
	return false;
}

bool board::canMove(int player, point from, point to, bool updateCheckStatus, bool updateCheckmate)
{
	char fromValue = state[from.R][from.C];
	char toValue = state[to.R][to.C];

	// General rules
	if (getItemOwner(fromValue) != player)
	{
		if (!updateCheckStatus && !updateCheckmate)
			cout << "It's not your item. ";
		return false;
	}
	if (getItemOwner(toValue) == getItemOwner(fromValue))
	{
		if (!updateCheckStatus && !updateCheckmate)
			cout << "You can't hit your items. ";
		return false;
	}
	if (!updateCheckStatus)
	{
		if (state[to.R][to.C] == 'k' || state[to.R][to.C] == 'K') // Can not hit king
		{
			if (!updateCheckmate)
				cout << "Can not hit king. ";
			return false;
		}
	}

	// Pawn
	if (fromValue == 'p' || fromValue == 'P')
		return canMovePawn(player, from, to);

	// Rook
	if (fromValue == 'r' || fromValue == 'R')
		return canMoveRook(player, from, to);

	// Horse
	if (fromValue == 'h' || fromValue == 'H')
		return canMoveHorse(player, from, to);

	// Elephant
	if (fromValue == 'e' || fromValue == 'E')
		return canMoveElephant(player, from, to);

	// Queen
	if (fromValue == 'q' || fromValue == 'Q')
		return canMoveQueen(player, from, to);

	// King
	if (fromValue == 'k' || fromValue == 'K')
		return canMoveKing(player, from, to);

	return false;
}

bool board::isChecked(int player, point kingPoint)
{
	int opponent = getOpponent(player);
	for (int r = 0; r < 8; r++)
	{
		for (int c = 0; c < 8; c++)
		{
			if (getItemOwner(state[r][c]) == opponent && canMove(opponent, point(r, c), kingPoint, true, false))
				return true;
		}
	}
	return false;
}

void board::updateCheckStatus()
{
	// Update Black Check Status
	point bKingPoint = getItemPoint('k');
	blackChecked = isChecked(BlackPlayer, bKingPoint);


	// Update White Check Status
	point wKingPoint = getItemPoint('K');
	whiteChecked = isChecked(WhitePlayer, wKingPoint);
}

bool board::isCheckmate(int player, bool checked, char king)
{
	if (!checked)
		return false;

	for (int r = 0; r < 8; r++)
	{
		for (int c = 0; c < 8; c++)
		{
			char fromValue = state[r][c];
			if (getItemOwner(fromValue) == player)
			{
				for (int i = 0; i < 8; i++)
				{
					for (int j = 0; j < 8; j++)
					{
						if (canMove(player, point(r, c), point(i, j), false, true))
						{
							char toValue = state[i][j];

							state[r][c] = ' ';
							state[i][j] = fromValue;

							bool checked = isChecked(player, getItemPoint(king));

							// Revert
							state[r][c] = fromValue;
							state[i][j] = toValue;

							if (!checked)
								return false;
						}
					}
				}
			}
		}
	}
	return true;
}

bool board::isStalemate(int player, char king)
{
	for (int r = 0; r < 8; r++)
	{
		for (int c = 0; c < 8; c++)
		{
			char fromValue = state[r][c];
			if (getItemOwner(fromValue) == player)
			{
				for (int i = 0; i < 8; i++)
				{
					for (int j = 0; j < 8; j++)
					{
						if (canMove(player, point(r, c), point(i, j), false, true))
						{
							char toValue = state[i][j];

							state[r][c] = ' ';
							state[i][j] = fromValue;

							bool checked = isChecked(player, getItemPoint(king));

							// Revert
							state[r][c] = fromValue;
							state[i][j] = toValue;

							if (!checked)
								return false;
						}
					}
				}
			}
		}
	}
	return true;
}

void board::promote(char fromValue, point to)
{
	if (fromValue == 'p')
	{
		// Black pawn
		if (to.R == 7)
			state[to.R][to.C] = 'q';
	}
	if (fromValue == 'P')
	{
		// White pawn
		if (to.R == 0)
			state[to.R][to.C] = 'Q';
	}
}

void board::print()
{
	// Clear board
	system("CLS");

	// Print Header
	cout << "   ";
	for (int c = 0; c < 8; c++)
		cout << "   " << c << "  ";
	cout << endl;

	cout << "   ";
	for (int c = 0; c < 8; c++)
		cout << " _____";
	cout << endl;

	// Print Rows
	for (int r = 0; r < 8; r++)
	{
		cout << "   ";
		for (int i = 0; i < 8; i++)
			cout << "|     ";
		cout << "|" << endl;

		cout << " " << r << " |";

		// Print Columns
		for (int c = 0; c < 8; c++)
			cout << "  " << state[r][c] << "  |";
		if (r == 0)
		{
			cout << "   BLACK PLAYER";
			if (blackCheckmate)
				cout << "   CHECKMATE";
			else if(blackChecked)
				cout << "   CHECKED";
			else if (blackStalemate)
				cout << "   STALEMATE";
		}
		else if (r == 7)
		{
			cout << "   WHITE PLAYER";
			if (whiteCheckmate)
				cout << "   CHECKMATE";
			else if (whiteChecked)
				cout << "   CHECKED";
			else if (whiteStalemate)
				cout << "   STALEMATE";
		}
		cout << endl;

		cout << "   ";
		for (int c = 0; c < 8; c++)
			cout << "|_____";
		cout << "| " << endl;
	}
}

int board::getOpponent(int player)
{
	if (player == BlackPlayer)
		return WhitePlayer;
	return BlackPlayer;
}

bool board::gameFinished()
{
	if (blackCheckmate || whiteCheckmate || blackStalemate || whiteStalemate)
		return true;
	return false;
}
