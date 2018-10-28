#include "Game.h"

namespace TicTacToe {


	//Recv Codes
	//0001 = Only one Player
	//0002 = Two players
	//0003 = You are the first player
	//0004 = You are the second player

	int bytesRecv;		 // amount of bytes that we've recieved
	SOCKET sock;		 // connection to server
	SOCKADDR_IN addr;	 // server informations
	char buffer[10];	 // contains messages from server
	char player;		 // The player mark (sign (X, O) )
	std::string ip;		 // Server IP	
	int port;			 // Server Port
	int pos;			 // players input pos
	std::string s_input; // player pos input - s_input is used to check if the players input is out of range

	// Startup winsock
	int startWinsock()
	{
		WSADATA wsa;

		return WSAStartup(MAKEWORD(2, 1), &wsa);
	}

	// User has to choose a field where is mark will be set
	void input()
	{
		do
		{
			std::cout << std::endl << "Choice: ";

			std::cin >> s_input;
			std::cin.clear();
			std::cin.ignore();

			if (s_input.length() > 1 || static_cast<int>(s_input[0]) < static_cast<int>('0') 
				|| static_cast<int>(s_input[0])> static_cast<int>('8'))
				input();

			pos = std::stoi(s_input);

		} while (std::cin.fail() || pos < 0 || pos > 8);
	}

	// Get gameboard from server and load it to clientside gameboard
	void LoadGameboard(Gameboard* board, char * buffer)
	{
		for (int i = 0; i < 9; i++)
			board->getBoard()[i] = buffer[i];
	}

	// Fill buffer with our edited gameboard
	void SetBuff(Gameboard* board, char * buffer)
	{
		for (int i = 0; i < 9; i++)
			buffer[i] = board->getBoard()[i];
	}

	// Server determines who is X and O
	void getPlayerMark()
	{
		bytesRecv = recv(sock, buffer, 10, 0);

		if (bytesRecv == SOCKET_ERROR || bytesRecv == 0)
		{
			std::cout << "Lost connection: " << WSAGetLastError() << std::endl;

			return;
		}
		else
		{
			buffer[bytesRecv - 1] = '\0';

			if (strcmp(buffer, "0003") == 0)
				player = 'X';
			else if (strcmp(buffer, "0004") == 0)
				player = 'O';
		}
		system("cls");
	}

	// Play against another player
	void MultiplayerGame()
	{
		Gameboard Board;

		while (true) {

			std::cout << "You are player '" << player << "'" << std::endl;

			Board.drawBoard();

			std::cout << "Wait for player input..." << std::endl;

			memset(buffer, 0, sizeof(buffer));

			bytesRecv = recv(sock, buffer, sizeof(buffer), 0);

			if (bytesRecv == SOCKET_ERROR || bytesRecv == 0)
			{
				std::cout << "Lost connection: " << WSAGetLastError() << std::endl;
				return;
			}

			buffer[bytesRecv - 1] = '\0';

			if (strcmp(buffer, "Leave") == 0)
			{
				system("cls");

				std::cout << "Player left game" << std::endl << std::endl;

				closesocket(sock);

				return;
			}

			LoadGameboard(&Board, buffer);

			system("cls");

			std::cout << "You are player '" << player << "'" << std::endl;

			Board.drawBoard();

			// Check if someone won
			if (Board.gameover())
			{
				std::cout << "\n" << Board.getWinner() << " won" << std::endl;

				closesocket(sock);

				break;
			}
			else if (Board.isBoardFull())
			{
				std::cout << "\n" << "Drawn " << std::endl;

				break;
			}

			do
			{
				input();
			} while (pos > 9 || pos < 0 || !Board.setField(pos, player));

			memset(buffer, 0, sizeof(buffer));

			SetBuff(&Board, buffer);

			bytesRecv = send(sock, buffer, sizeof(buffer), 0);

			if (bytesRecv == SOCKET_ERROR || bytesRecv == 0)
			{
				std::cout << "Lost connection: " << WSAGetLastError() << std::endl;

				return;
			}

			system("cls");

			std::cout << "You are player '" << player << "'" << std::endl;

			Board.drawBoard();

			// Prüfe ob jmd gewonnen hat
			if (Board.gameover())
			{
				std::cout << "\n" << Board.getWinner() << " won" << std::endl;

				closesocket(sock);

				break;
			}
			else if (Board.isBoardFull())
			{
				std::cout << "\n" << "Drawn " << std::endl;

				break;
			}

			system("cls");
		}
	}

	// Check if there is no emtpy field anymore
	bool isBoardFull(Gameboard& board)
	{
		for (int i = 0; i < 9; i++)
		{
			if (board.getBoard()[i] != 'X' && board.getBoard()[i] != 'O')
				return false;
		}
		return true;
	}

	// Play against the computer
	void SingleplayerGame()
	{
		Gameboard Board;

		Enemy Computer(Board, 'O', 'X');

		while (true)
		{
			system("cls");
			Board.drawBoard();

			do
			{
				std::cout << "Choice:";

				std::cin >> pos;
				std::cin.ignore();
				std::cin.clear();

			} while (pos > 9 || pos < 0 || !Board.setField(pos, 'X'));

			system("cls");

			Board.drawBoard();

			if (Board.gameover())
			{
				std::cout << "\n" << Board.getWinner() << " won" << std::endl;

				break;
			}
			else if (isBoardFull(Board))
			{
				std::cout << "\nDrawn!" << std::endl;

				break;
			}

			Computer.setField();

			system("cls");

			Board.drawBoard();

			if (Board.gameover())
			{
				std::cout << "\n" << Board.getWinner() << " won" << std::endl;

				break;
			}
		}
	}

	// Connect to the server
	bool buildConnectionToServer()
	{

		sock = socket(AF_INET, SOCK_STREAM, 0);

		if (sock == INVALID_SOCKET)
		{
			std::cout << "Couldn't create a connection: " << WSAGetLastError() << std::endl;

			return false;
		}

		else
		{
			memset(&addr, 0, sizeof(SOCKADDR_IN));

			addr.sin_family = AF_INET;
			addr.sin_port = htons(port);
			addr.sin_addr.s_addr = inet_addr(ip.c_str());

			bytesRecv = connect(sock, (SOCKADDR*)&addr, sizeof(SOCKADDR));

			if (bytesRecv == SOCKET_ERROR)
			{
				std::cout << "Couldn't build connection: " << WSAGetLastError() << std::endl;

				return false;
			}

			else
			{
				std::cout << "Waiting for one more player..." << std::endl;

				do
				{
					memset(buffer, 0, strlen(buffer));

					bytesRecv = recv(sock, buffer, 10, 0);

					if (bytesRecv == SOCKET_ERROR)
					{
						system("cls");

						std::cout << "Lost connection: " << WSAGetLastError() << std::endl;

						closesocket(sock);

						return false;
					}
					else if (bytesRecv == 0)
					{
						system("cls");

						std::cout << "Lost connection: " << WSAGetLastError() << std::endl;

						closesocket(sock);

						return false;
					}
					else if (std::strcmp(buffer, "Leave") == 0)
					{
						system("cls");

						std::cout << "Player left" << std::endl << std::endl;

						closesocket(sock);

						return false;
					}

				} while (strcmp(buffer, "0002") != 0);

				return true;
			}
		}

		return false;
	}

	void PlayMultiplayer()
	{
		bytesRecv = startWinsock();

		if (bytesRecv != 0)
		{
			std::cout << "WSA Startup error: " << WSAGetLastError() << std::endl;

			return;
		}

		std::cout << "IP: ";
		std::getline(std::cin, ip);
		std::cin.clear();

		port = 58264;

		if (buildConnectionToServer())
		{
			getPlayerMark();

			MultiplayerGame();
		}
	}
	// Draw in the center of the screen with an offset y

	void Menu()
	{
		int choice;

		while (true)
		{
			std::cout << "[0]Single Player" << std::endl;
			std::cout << "[1]Multiplayer" << std::endl;
			std::cout << "[2]Quit" << std::endl;

			do
			{
				std::cout << "Choice:";

				std::cin >> choice;
				std::cin.clear();
				std::cin.ignore();

			} while (std::cin.fail() || choice < 0 || choice > 2);

			switch (choice)
			{
			case 0:

				SingleplayerGame();

				break;
			case 1:

				PlayMultiplayer();

				break;
			case 2:
				exit(0);
			}
		}
	}


}