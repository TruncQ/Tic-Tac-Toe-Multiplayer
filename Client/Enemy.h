#pragma once
#include "Gameboard.h"
#include <time.h>
#include <random>

class Enemy
{
public:
	Enemy(Gameboard&, char _me, char _player);	// Setup Player and Enemy
	bool CanWin(char _player);					// Am I close to win?
	void setField();							// Set (O to) a position
	void randomNumber();						// Create a random number (0 - 8)
private:
	Gameboard & Board;		// Contains the gamefield
	char player, me;		// me = Computer, player = User
	int pos;				// Position where I (Enemy) can set(O)
};
