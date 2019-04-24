#pragma once
#pragma comment( lib, "ws2_32.lib" )
#include <winsock2.h> // bei manchan compilern muss man nur windows.h includieren (zB VC++)
#include <windows.h>
#include <iostream>
#include <thread>
#include <random>
#include <ctime>
#define MAX_CLIENTS 100
#define MIN_CLIENTS 2
namespace Server {
	int startWinsock();														// Setup Winsock
	void StartGame(SOCKET(_clients)[MAX_CLIENTS][MIN_CLIENTS], int _pos);	// Two players play against each other
	void PrepareServer();													// Start the server
}