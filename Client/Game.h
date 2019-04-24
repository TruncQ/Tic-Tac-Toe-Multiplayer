#pragma once
#include "Gameboard.h"
#include "Enemy.h" 
#include <string>
#include <WinSock2.h>
#include <Windows.h>
#pragma comment( lib, "ws2_32.lib" )
namespace TicTacToe
{
	
	extern int pos;				// players input pos
	extern std::string s_input;	// player pos input - s_input is used to check if the players input is out of range
	//Recv Codes
	//0001 = Only one Player
	//0002 = Two players
	//0003 = You are the first player
	//0004 = You are the second player
	
	extern int bytesRecv;									// amount of bytes that we recieved
	extern SOCKET sock;										// connection to server
	extern SOCKADDR_IN addr;								// server informations
	extern char buffer[10];									// contains messages from server
	extern char player;										// The player mark (sign (X, O) )
	extern std::string ip;									// Server IP
	extern int port;										// Server Port
	int startWinsock();										// Startup winsock
	void input();											// User has to choose a field where is mark will be set
	void LoadGameboard(Gameboard* board, char * buffer);	// Get gameboard from server and load it to clientside gameboard
	void SetBuff(Gameboard* board, char * buffer);			// Fill buffer with our edited gameboard
	void getPlayerMark();									// Server determines who is X and O
	void MultiplayerGame();									// Play against another player
	void PlayMultiplayer();
	void Menu();											// Draw Menu 
	void SingleplayerGame();								// Play against the computer
	bool isBoardFull(Gameboard& board);						// Check if there is no emtpy field anymore
	bool buildConnectionToServer();							// Connect to the server
}	