#pragma once
#include <vector>
#include <iostream>

#include "chess.hpp"

//std::vector<int> evens = { 2, 4, 6, 8 };
//std::vector<int> odds = { 1, 3, 5, 7, 9 };

enum PieceValues
{
	EMPTY = 0,
	PAWN = 1,
	ROOK = 5,
	KNIGHT = 3,
	BISHOP = 3,
	QUEEN = 9,
	KING = 500,
};

//class Move
//{
//public:
//	int square;
//	int piece;
//
//	/*bool operator==(const Move& other)
//	{
//		return (square == other.square) && (piece == other.piece);
//	}*/
//};
//
//class ChessMove : public Move
//{
//public:
//	ChessMove()
//	{
//		square = 0;
//		piece = 0;
//	}
//
//	ChessMove(int square, int piece)
//	{
//		this->square = square;
//		this->piece = piece;
//	}
//
//	bool operator==(const ChessMove& other)
//	{
//		return (square == other.square) && (piece == other.piece);
//	}
//};
//
//// numbers can from 0 to 9 so we need 4 bits for every number
//struct Chess
//{
//	// 64 bits to store the whole board
//	// 9 numbers * 4 bits = only 36 bits are required
//	uint64_t board = 0;
//
//	// numbers can go from 0 to 9
//	uint8_t get(size_t index)
//	{
//		return (board >> (index * 4)) & 0b1111;
//	}
//
//	void set(size_t index, uint8_t value)
//	{
//		board &= ~(0b1111ull << (index * 4));
//		board |= (uint64_t(value) << (index * 4));
//	}
//
//	void operator=(const Chess& rhs)
//	{
//		board = rhs.board;
//	}
//
//	bool operator==(const Chess& rhs)
//	{
//		return board == rhs.board;
//	}
//
//	friend bool operator==(const Chess& lhs, const Chess& rhs)
//	{
//		return lhs.board == rhs.board;
//	}
//
//	friend std::ostream& operator<<(std::ostream& out, const Chess& board)
//	{
//		out << board.board;
//		return out;
//	}
//};


//class GenericBoard
//{
//public:
//	bool player; // true = even, false = odd
//
//	virtual chess::Board MakeMove(chess::Move move) = 0;
//
//	virtual std::vector<chess::Move> LegalMoves() = 0;
//
//	virtual bool IsWin() = 0;
//	virtual bool IsDraw() = 0;
//	virtual float Evaluate(bool player) = 0;
//	virtual void DisplayBoard() = 0;
//};

class ChessBoard
{
public:
	//Chess board;
	chess::Board board;

	ChessBoard()
	{
		/*for (int i = 0; i < 9; i++)
			board.set(i, 0);

		player = false;*/
	}

	ChessBoard(chess::Board b)
	{
		board = b;
	}

	/*ChessBoard(bool player)
	{
		for (int i = 0; i < 9; i++)
			board.set(i, 0);

		this->player = player;
	}

	ChessBoard(Chess board, bool player)
	{
		this->board = board;
		this->player = player;
	}*/

	ChessBoard MakeMove(chess::Move move)
	{
		ChessBoard newBoard = ChessBoard(board);
		newBoard.board.makeMove(move);

		return newBoard;
	}

	chess::Movelist LegalMoves()
	{
		chess::Movelist moveList;
		chess::movegen::legalmoves(moveList, board);
		
		return moveList;
	}

	int NumAvailableAttacks(chess::Color player)
	{
		//if (player == chess::Color::WHITE)
		//std::cout << board.getFen() << std::endl;

		std::string fen = board.getFen();
		int numAttacks = 0, boardPos = 0;
		char ch;
		for (int i = 0; boardPos < 64; i++)
		{
			ch = fen[i];

			if (std::isdigit(ch))// If it is a number, skip
				boardPos += int(ch - '0');
			else if (ch != '/')
			{
				if ((!std::isupper(ch) && player == chess::Color::WHITE) || (std::isupper(ch) && player == chess::Color::BLACK)) // If White check no white spaces are attacked
				{
					if (board.isAttacked(chess::Square(boardPos), player))
					{
						ch = std::tolower(ch);

						switch (ch)
						{
						case 'p':
							numAttacks += PAWN;
							break;
						case 'r':
							numAttacks += ROOK;
							break;
						case 'n':
							numAttacks += KNIGHT;
							break;
						case 'b':
							numAttacks += BISHOP;
							break;
						case 'q':
							numAttacks += QUEEN;
							break;
						case 'k':
							numAttacks += KING;
							break;
						default:
							std::cout << "Default, not good\n";
						}
					}
				}
				//else if (std::isupper(ch) && player == chess::Color::BLACK)
				//{
				//	if (board.isAttacked(chess::Square(boardPos), player))
				//		numAttacks++;
				//}
					//board.isAttacked(chess::Square(boardPos), player);
				boardPos++;
			}
		}
		//std::cout << numAttacks << std::endl;
		return numAttacks;
	}

	bool IsWin()
	{
		auto result = board.isGameOver();

		return result.second == chess::GameResult::WIN;
	}

	bool IsDraw()
	{
		auto result = board.isGameOver();

		return result.second == chess::GameResult::DRAW;
	}

	float Evaluate(chess::Color player)
	{
		//TODO
		//Implement an evaluation function
		float val = 0;

		//Summed score of our remaining pieces
		/*val += board.pieces(chess::PieceType::PAWN, player).count() * PieceValues::PAWN;
		val += board.pieces(chess::PieceType::ROOK, player).count() * PieceValues::ROOK;
		val += board.pieces(chess::PieceType::KNIGHT, player).count() * PieceValues::KNIGHT;
		val += board.pieces(chess::PieceType::BISHOP, player).count() * PieceValues::BISHOP;
		val += board.pieces(chess::PieceType::QUEEN, player).count() * PieceValues::QUEEN;
		val += board.pieces(chess::PieceType::KING, player).count() * PieceValues::KING;*/

		//Summed score of opponent's remaining pieces subtracted from ours
		chess::Color otherCol = !player;
		/*val -= board.pieces(chess::PieceType::PAWN, otherCol).count() * PieceValues::PAWN;
		val -= board.pieces(chess::PieceType::ROOK, otherCol).count() * PieceValues::ROOK;
		val -= board.pieces(chess::PieceType::KNIGHT, otherCol).count() * PieceValues::KNIGHT;
		val -= board.pieces(chess::PieceType::BISHOP, otherCol).count() * PieceValues::BISHOP;
		val -= board.pieces(chess::PieceType::QUEEN, otherCol).count() * PieceValues::QUEEN;
		val -= board.pieces(chess::PieceType::KING, otherCol).count() * PieceValues::KING;*/
		
		//val += LegalMoves().size();
		val = (PieceValues::KING * (board.pieces(chess::PieceType::KING, player).count() - board.pieces(chess::PieceType::KING, otherCol).count())) +
			(PieceValues::QUEEN * (board.pieces(chess::PieceType::QUEEN, player).count() - board.pieces(chess::PieceType::QUEEN, otherCol).count())) +
			(PieceValues::ROOK * (board.pieces(chess::PieceType::ROOK, player).count() - board.pieces(chess::PieceType::ROOK, otherCol).count())) +
			(PieceValues::BISHOP * (board.pieces(chess::PieceType::BISHOP, player).count() - board.pieces(chess::PieceType::BISHOP, otherCol).count())) +
			(PieceValues::KNIGHT * (board.pieces(chess::PieceType::KNIGHT, player).count() - board.pieces(chess::PieceType::KNIGHT, otherCol).count())) +
			(PieceValues::PAWN * (board.pieces(chess::PieceType::PAWN, player).count() - board.pieces(chess::PieceType::PAWN, otherCol).count())) +
			(0.1f * LegalMoves().size());

		//Check number of attacks both side can make
		//val -= NumAvailableAttacks(player);
		//val += NumAvailableAttacks(otherCol);
		//std::cout << val << "\n";

		return val;
	}

	bool operator==(const ChessBoard& rhs) const
	{
		return board.occ() == rhs.board.occ();
	}
};

namespace std
{
	template<>
	struct hash<ChessBoard>
	{
		size_t operator()(const ChessBoard& board) const
		{
			return board.board.hash();
		}
	};
}