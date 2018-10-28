#pragma once
#include <iostream>
#include <vector>

class Gameboard
{
public:
	bool setField(int _pos, char _player);	// set player mark at position
	bool gameover();						// Check if someone has won
	const char getWinner();					// return winner
	const bool isBoardFull();				// Check if gameboard is full
	void drawBoard();						// Draw board
	Gameboard();							// Set gameboard up by default settings
	std::vector<char>& getBoard();			// Return Reference to Gameboard
private:
	
	std::vector<char> Board;				// Gameboard is saved in here
	char winner;							// Save the winner
};
