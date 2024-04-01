#pragma once
#include <vector>
#include <iostream>

std::vector<int> evens = { 2, 4, 6, 8 };
std::vector<int> odds = { 1, 3, 5, 7, 9 };

class Move
{
public:
	int square, piece;

	/*bool operator==(const Move& other)
	{
		return (square == other.square) && (piece == other.piece);
	}*/
};

class NTTTMove : public Move
{
public:
	NTTTMove()
	{
		square = 0;
		piece = 0;
	}

	NTTTMove(int square, int piece)
	{
		this->square = square;
		this->piece = piece;
	}

	bool operator==(const NTTTMove& other)
	{
		return (square == other.square) && (piece == other.piece);
	}
};

// numbers can from 0 to 9 so we need 4 bits for every number
struct NTTT
{
	// 64 bits to store the whole board
	// 9 numbers * 4 bits = only 36 bits are required
	uint64_t board = 0;

	// numbers can go from 0 to 9
	uint8_t get(size_t index)
	{
		return (board >> (index * 4)) & 0b1111;
	}

	void set(size_t index, uint8_t value)
	{
		board &= ~(0b1111ull << (index * 4));
		board |= (uint64_t(value) << (index * 4));
	}

	void operator=(const NTTT& rhs)
	{
		board = rhs.board;
	}

	bool operator==(const NTTT& rhs)
	{
		return board == rhs.board;
	}

	friend bool operator==(const NTTT& lhs, const NTTT& rhs)
	{
		return lhs.board == rhs.board;
	}

	friend std::ostream& operator<<(std::ostream& out, const NTTT& board)
	{
		out << board.board;
		return out;
	}
};

namespace std
{
	template<>
	struct hash<NTTT>
	{
		size_t operator()(const NTTT& nttt) const
		{
			return std::hash<uint64_t>()(nttt.board);
		}
	};
}

template <typename move_t, typename board_t>
requires std::is_same<move_t, NTTTMove>::value
class Board
{
public:
	bool player; // true = even, false = odd

	board_t MakeMove(move_t move);

	std::vector<move_t> LegalMoves();

	bool IsWin();
	bool IsDraw();
	float Evaluate(bool player);
	void DisplayBoard();
};

class NTTTBoard : public Board<NTTTMove, NTTTBoard>
{
public:
	NTTT board;

	NTTTBoard()
	{
		for (int i = 0; i < 9; i++)
			board.set(i, 0);

		player = false;
	}

	NTTTBoard(bool player)
	{
		for (int i = 0; i < 9; i++)
			board.set(i, 0);

		this->player = player;
	}

	NTTTBoard(NTTT board, bool player)
	{
		this->board = board;
		this->player = player;
	}

	NTTTBoard MakeMove(NTTTMove move)
	{
		NTTT newBoard = board;
		newBoard.set(move.square, move.piece);

		return NTTTBoard(newBoard, !player);
	}

	std::vector<NTTTMove> LegalMoves()
	{
		std::vector<int> even = evens;
		std::vector<int> odd = odds;

		std::vector<NTTTMove> moves;
		std::vector<int> empty;

		for (int i = 0; i < 9; i++)
		{
			if ((int)board.get(i) != 0)
			{
				if ((int)board.get(i) % 2 == 0)
					even.erase(std::remove(even.begin(), even.end(), (int)board.get(i)), even.end());
				else
					odd.erase(std::remove(odd.begin(), odd.end(), (int)board.get(i)), odd.end());
			}
			else
				empty.push_back(i);
		}

		for (int i = 0; i < empty.size(); i++)
		{
			if (player)
			{
				for (int j = 0; j < even.size(); j++)
					moves.push_back(NTTTMove(empty[i], even[j]));
			}
			else
			{
				for (int j = 0; j < odd.size(); j++)
					moves.push_back(NTTTMove(empty[i], odd[j]));
			}
		}

		return moves;
	}

	bool IsWin()
	{
		//check diagonal
		if ((int)board.get(0) + (int)board.get(4) + (int)board.get(8) == 15 && (int)board.get(0) > 0 && (int)board.get(4) > 0 && (int)board.get(8) > 0)
			return true;
		if ((int)board.get(2) + (int)board.get(4) + (int)board.get(6) == 15 && (int)board.get(2) > 0 && (int)board.get(4) > 0 && (int)board.get(6) > 0)
			return true;

		//check horizontal
		if ((int)board.get(0) + (int)board.get(1) + (int)board.get(2) == 15 && (int)board.get(0) > 0 && (int)board.get(1) > 0 && (int)board.get(2) > 0)
			return true;
		if ((int)board.get(3) + (int)board.get(4) + (int)board.get(5) == 15 && (int)board.get(3) > 0 && (int)board.get(4) > 0 && (int)board.get(5) > 0)
			return true;
		if ((int)board.get(6) + (int)board.get(7) + (int)board.get(8) == 15 && (int)board.get(6) > 0 && (int)board.get(7) > 0 && (int)board.get(8) > 0)
			return true;

		//check vertical
		if ((int)board.get(0) + (int)board.get(3) + (int)board.get(6) == 15 && (int)board.get(0) > 0 && (int)board.get(3) > 0 && (int)board.get(6) > 0)
			return true;
		if ((int)board.get(1) + (int)board.get(4) + (int)board.get(7) == 15 && (int)board.get(1) > 0 && (int)board.get(4) > 0 && (int)board.get(7) > 0)
			return true;
		if ((int)board.get(2) + (int)board.get(5) + (int)board.get(8) == 15 && (int)board.get(2) > 0 && (int)board.get(5) > 0 && (int)board.get(8) > 0)
			return true;

		return false;
	}

	int NumEmptySpace()
	{
		int numEmpty = 0;
		for (int i = 0; i < 9; i++)
		{
			if (board.get(i) == 0)
				numEmpty++;
		}

		return numEmpty;
	}

	bool IsDraw()
	{
		return (!IsWin() && LegalMoves().size() == 0);
	}

	float Evaluate(bool player)
	{
		if (IsWin() && this->player == player)
			return -1.0f * (NumEmptySpace() + 1);
		else if (IsWin() && this->player != player)
			return (NumEmptySpace() + 1);
		else
			return 0;
	}

	void DisplayBoard()
	{
		int column = 0;
		std::cout << "-----------" << std::endl;
		for (int i = 0; i < 9; i++)
		{
			if (/*boardPosition[i]*/(int)board.get(i) == 0)
				std::cout << "  ";
			else
				std::cout << " " << /*boardPosition[i]*/(int)board.get(i);

			if (column >= 2)
			{
				column = 0;
				std::cout << std::endl;
				std::cout << "-----------" << std::endl;
			}
			else
			{
				column++;
				std::cout << " |";
			}
		}
	}
};