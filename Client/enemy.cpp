#include "Enemy.h"


Enemy::Enemy(Gameboard& board, char _me, char _player) : Board(board), me(_me), player(_player), pos(0) {}

void Enemy::setField()
{
	// Check if Computer can win
	if (CanWin(me))
		Board.setField(pos, me);

	// Check if User can win
	else if (CanWin(player))
		Board.setField(pos, me);

	else
	{
		// If no-one can win set at random position
		while (!Board.setField(pos, me))
			randomNumber();
	}
}

bool Enemy::CanWin(char _player)
{
	int fieldsSet = 0;	// Check if player 2 fields in a raw already set
	int freeFields = 0;	// Check if there are free positions in the raw
	int _pos;			// Saves the position where player can set to win

						// Check first raw
	for (int i = 0; i <= 2; i++)
	{
		if (Board.getBoard()[i] == _player)
			++fieldsSet;

		if (int(Board.getBoard()[i]) >= 48 && int(Board.getBoard()[i]) <= 57)
		{
			_pos = i;

			++freeFields;
		}
	}

	if (fieldsSet == 2 && freeFields == 1)
	{
		pos = _pos;

		return true;
	}
	else
	{
		fieldsSet = 0;

		freeFields = 0;
	}

	// Check second raw
	for (int i = 3; i <= 5; i++)
	{
		if (Board.getBoard()[i] == _player)
			++fieldsSet;

		if (int(Board.getBoard()[i]) >= 48 && int(Board.getBoard()[i]) <= 57)
		{
			_pos = i;

			++freeFields;
		}
	}

	if (fieldsSet == 2 && freeFields == 1)
	{
		pos = _pos;

		return true;
	}
	else
	{
		fieldsSet = 0;

		freeFields = 0;
	}

	// Check third raw
	for (int i = 6; i <= 8; i++)
	{
		if (Board.getBoard()[i] == _player)
			++fieldsSet;

		if (int(Board.getBoard()[i]) >= 48 && int(Board.getBoard()[i]) <= 57)
		{
			_pos = i;

			++freeFields;
		}
	}

	if (fieldsSet == 2 && freeFields == 1)
	{
		pos = _pos;

		return true;
	}
	else
	{
		fieldsSet = 0;

		freeFields = 0;
	}

	// ----------- Check column -----------
	for (int i = 0; i <= 6;)	// First column
	{
		if (Board.getBoard()[i] == _player)
			++fieldsSet;

		if (int(Board.getBoard()[i]) >= 48 && int(Board.getBoard()[i]) <= 57)
		{
			_pos = i;

			++freeFields;
		}
		i += 3;
	}

	if (fieldsSet == 2 && freeFields == 1)
	{
		pos = _pos;

		return true;
	}
	else
	{
		fieldsSet = 0;

		freeFields = 0;
	}

	for (int i = 1; i <= 7;)	// Second Column
	{
		if (Board.getBoard()[i] == _player)
			++fieldsSet;

		if (int(Board.getBoard()[i]) >= 48 && int(Board.getBoard()[i]) <= 57)
		{
			_pos = i;

			++freeFields;
		}
		i += 3;
	}

	if (fieldsSet == 2 && freeFields == 1)
	{
		pos = _pos;

		return true;
	}
	else
	{
		fieldsSet = 0;

		freeFields = 0;
	}


	for (int i = 2; i <= 8;)	// Third Column
	{
		if (Board.getBoard()[i] == _player)
			++fieldsSet;

		if (int(Board.getBoard()[i]) >= 48 && int(Board.getBoard()[i]) <= 57)
		{
			_pos = i;

			++freeFields;
		}
		i += 3;
	}

	if (fieldsSet == 2 && freeFields == 1)
	{
		pos = _pos;

		return true;
	}
	else
	{
		fieldsSet = 0;

		freeFields = 0;
	}

	// ----------- Check crosswise -----------
	for (int i = 0; i <= 8;)	// From left to right
	{
		if (Board.getBoard()[i] == _player)
			++fieldsSet;

		if (int(Board.getBoard()[i]) >= 48 && int(Board.getBoard()[i]) <= 57)
		{
			_pos = i;

			++freeFields;
		}
		i += 4;
	}

	if (fieldsSet == 2 && freeFields == 1)
	{
		pos = _pos;

		return true;
	}
	else
	{
		fieldsSet = 0;

		freeFields = 0;
	}


	for (int i = 2; i <= 6;)	// From right to left
	{
		if (Board.getBoard()[i] == _player)
			++fieldsSet;

		if (int(Board.getBoard()[i]) >= 48 && int(Board.getBoard()[i]) <= 57)
		{
			_pos = i;

			++freeFields;
		}
		i += 2;
	}

	if (fieldsSet == 2 && freeFields == 1)
	{
		pos = _pos;

		return true;
	}

	return false;
}

void Enemy::randomNumber()
{
	std::default_random_engine generator(time(nullptr));
	std::uniform_int_distribution<int> distribution(0, 8);

	pos = distribution(generator);
}
