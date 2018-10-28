#include "Server.h"


namespace Server 
{
	// Setup Winsock
	int startWinsock(void)
	{
		WSADATA wsa;

		return WSAStartup(MAKEWORD(2, 0), &wsa);
	}

	// Two players play against each other
	void StartGame(SOCKET(_clients)[MAX_CLIENTS][MIN_CLIENTS], int _pos)
	{
		int bytesRecv = 0;
		int change = 0;
		char buffer[10] = { "012345678" };
		char buffer2[10];

		memset(buffer2, 0, sizeof(buffer2));

		strcpy(buffer2, "0003");

		send(_clients[_pos][0], buffer2, sizeof(buffer2), 0);

		memset(buffer2, 0, sizeof(buffer2));

		strcpy(buffer2, "0004");

		send(_clients[_pos][1], buffer2, sizeof(buffer2), 0);

		// Random beginner 
		std::default_random_engine generator(time(nullptr));
		std::uniform_int_distribution<int> distribution(0, 1);

		change = distribution(generator);

		while (true) {

			bytesRecv = send(_clients[_pos][change], buffer, sizeof(buffer), 0);

			memset(buffer, 0, sizeof(buffer));

			bytesRecv = recv(_clients[_pos][change], buffer, sizeof(buffer), 0);

			if (bytesRecv == SOCKET_ERROR || bytesRecv == 0) 
			{
				send(_clients[_pos][!change], "Leave", 6, 0);

				_clients[_pos][change]  = INVALID_SOCKET;
				_clients[_pos][!change] = INVALID_SOCKET;

				closesocket(_clients[_pos][change]);
				closesocket(_clients[_pos][!change]);

				std::cout << "Players left game" << std::endl;

				break;
			}/*
			else if (bytesRecv == 0) 
			{
				send(_clients[_pos][!change], "Leave", 6, 0);

				_clients[_pos][change]  = INVALID_SOCKET;
				_clients[_pos][!change] = INVALID_SOCKET;

				closesocket(_clients[_pos][change]);
				closesocket(_clients[_pos][!change]);

				std::cout << "Players left game" << std::endl;

				break;
			}
*/
			// Switch player 
			if (change == 0)
				change = 1;
			else if (change == 1)
				change = 0;
		}
	}

	// Start Server
	void PrepareServer()
	{
		std::thread games[1000];

		memset(games, 0, sizeof(games));

		short count = 0;		// Amount of connections
		int port = 58264;		// Server Port
		int bytesRecv;			// Data transfer controller
		SOCKET acceptSocket;	// New connections
		SOCKADDR_IN addr;		// Server Infos
		char buffer[10];		// Buffer
		FD_SET fdSet;			
		SOCKET clients[MAX_CLIENTS][MIN_CLIENTS];
		int i;

		bytesRecv = startWinsock();

		if (bytesRecv != 0)
		{
			std::cout << "Error: " << WSAGetLastError() << std::endl;

			return;
		}

		acceptSocket = socket(AF_INET, SOCK_STREAM, 0);

		if (acceptSocket == INVALID_SOCKET)
		{
			std::cout << "Error " << WSAGetLastError() << std::endl;

			return;
		}

		memset(&addr, 0, sizeof(SOCKADDR_IN));

		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		addr.sin_addr.s_addr = INADDR_ANY;

		bytesRecv = bind(acceptSocket, (SOCKADDR*)&addr, sizeof(SOCKADDR_IN));

		if (bytesRecv == SOCKET_ERROR)
		{
			std::cout << "Error: " << WSAGetLastError() << std::endl;

			return;
		}

		bytesRecv = listen(acceptSocket, 10);

		if (bytesRecv == SOCKET_ERROR)
		{
			std::cout << "Error: " << WSAGetLastError() << std::endl;;

			return;
		}
		else
			std::cout << "Server online!" << std::endl;
		
		for (i = 0; i < MAX_CLIENTS; i++)
		{
			for (int j = 0; j < MIN_CLIENTS; j++)
				clients[i][j] = INVALID_SOCKET;
		}

		while (true)
		{
		Begin:
			FD_ZERO(&fdSet);
			FD_SET(acceptSocket, &fdSet);

			for (i = 0; i < MAX_CLIENTS; i++)
			{
				for (int j = 0; j < 2; j++) 
				{
					if (clients[i][j] != INVALID_SOCKET)
						FD_SET(clients[i][j], &fdSet);
				}
			}

			bytesRecv = select(0, &fdSet, NULL, NULL, NULL);

			if (bytesRecv == SOCKET_ERROR)
			{
				std::cout << "Error: " << WSAGetLastError() << std::endl;

				return;
			}

			if (FD_ISSET(acceptSocket, &fdSet)) 
			{
				// If new connection comes up put it in fd
				for (i = 0; i < MAX_CLIENTS; i++)
				{
					for (int j = 0; j < 2; j++) 
					{
						// Find place to hold the new connection in Array
						if (clients[i][j] == INVALID_SOCKET)
						{
							clients[i][j] = accept(acceptSocket, NULL, NULL);

							std::cout << "Player joined server" << std::endl;

							

							// If there are 2 new connections let them play together
							if (clients[i][0] != INVALID_SOCKET && clients[i][1] != INVALID_SOCKET) 
							{
								++count;
								for (int c = 0; c < 2; c++)
								{
									memset(buffer, 0, sizeof(buffer));

									strcpy(buffer, "0002");

									bytesRecv = send(clients[i][c], buffer, sizeof(buffer), 0);

									if (bytesRecv == SOCKET_ERROR || bytesRecv == 0) 
									{
										send(clients[i][!c], "Leave", 6, 0);

										closesocket(clients[i][0]);
										closesocket(clients[i][1]);

										clients[i][0] = INVALID_SOCKET;
										clients[i][1] = INVALID_SOCKET;

										std::cout << "Players left" << std::endl;

										goto Begin;
									}
								}
								games[count] = std::thread(StartGame, clients, i);
							}
						}
					}
				}
			}
		}

		// Check if any client closed the connection
		for (int i = 0; i < MAX_CLIENTS; i++) 
		{
			for (int j = 0; j < MIN_CLIENTS; j++) 
			{
				if (clients[i][j] == INVALID_SOCKET) 
					continue;

				if (FD_ISSET(clients[i][j], &fdSet)) 
				{
					bytesRecv = recv(clients[i][j], buffer, 10, 0);

					if (bytesRecv == 0 || bytesRecv == SOCKET_ERROR) 
					{
						send(clients[i][0], "Leave", 6, 0);
						send(clients[i][1], "Leave", 6, 0);

						closesocket(clients[i][0]);
						closesocket(clients[i][1]);

						clients[i][0] = INVALID_SOCKET;
						clients[i][1] = INVALID_SOCKET;
						break;
					}
				}
			}
		}
	}
}