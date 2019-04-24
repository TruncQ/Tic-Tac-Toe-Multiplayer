#include "Gameboard.h"
// return winner
const char Gameboard::getWinner() { return winner; }
// return reference to gameboard
std::vector<char>& Gameboard::getBoard() { return Board; }
const bool Gameboard::isBoardFull() 
{
	for (int i = 0; i < 9; i++) 
	{
		if (int(Board[i]) >= 48 && int(Board[i]) <= 57)
			return false;
		
	}
	return true;
}
// Check if someone won 
bool Gameboard::gameover()
{
	// Check first raw
	if ((Board[0] == Board[1]) && Board[1] == Board[2]) 
	{
		winner = Board[0];
		return true;
	}
	// Check second raw
	else if ((Board[3] == Board[4]) && Board[4] == Board[5]) 
	{
		winner = Board[3];
		return true;
	}
	// Check third raw
	else if ((Board[6] == Board[7]) && Board[7] == Board[8]) 
	{
		winner = Board[6];
		return true;
	}
	// Check first column
	else if ((Board[0] == Board[3]) && Board[3] == Board[6]) 
	{
		winner = Board[0];
		return true;
	}
	// Check second column
	else if ((Board[1] == Board[4]) && Board[4] == Board[7])
	{
		winner = Board[1];
		return true;
	}
	// Check third column
	else if ((Board[2] == Board[5]) && Board[5] == Board[8]) 
	{
		winner = Board[2];
		return true;
	}
	// Check crosswise from left to right
	else if ((Board[0] == Board[4]) && Board[4] == Board[8]) 
	{
		winner = Board[0];
		return true;
	}
	// Check crosswise from right to left
	else if ((Board[2] == Board[4]) && Board[4] == Board[6]) 
	{
		winner = Board[2];
		return true;
	}
	return false;
}
bool Gameboard::setField(int _pos, char _player)
{
	// Check if position is not set already if so set position 
	if (int(Board[_pos]) >= 48 && int(Board[_pos]) <= 57)
	{
		Board[_pos] = _player;
		return true;
	}
	return false;
}
void Gameboard::drawBoard() 
{
	// Print out gameboard
	// Counter to display only 3 fields in a raw
	int c = 0;
	for (int i = 0; i <= 8; i++)
	{
		if (c % 3 == 0)
			std::cout << std::endl << std::endl;
		std::cout << Board[i] << "\t";
		++c;
	}
}
// Setup Gameboard
Gameboard::Gameboard() : winner(' ')
{
	for (int i = 0; i <= 8; i++)
		Board.push_back('0' + i);
}