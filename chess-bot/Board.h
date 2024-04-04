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

		int numAttacks = 0;
		for (int i = 0; i < 64; i++)
			if (board.isAttacked(chess::Square(i), player))
				numAttacks++;
		
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
		val += board.pieces(chess::PieceType::PAWN, player).count() * PieceValues::PAWN;
		val += board.pieces(chess::PieceType::ROOK, player).count() * PieceValues::ROOK;
		val += board.pieces(chess::PieceType::KNIGHT, player).count() * PieceValues::KNIGHT;
		val += board.pieces(chess::PieceType::BISHOP, player).count() * PieceValues::BISHOP;
		val += board.pieces(chess::PieceType::QUEEN, player).count() * PieceValues::QUEEN;
		val += board.pieces(chess::PieceType::KING, player).count() * PieceValues::KING;

		//Summed score of opponent's remaining pieces subtracted from ours
		chess::Color otherCol = !player;
		val -= board.pieces(chess::PieceType::PAWN, otherCol).count() * PieceValues::PAWN;
		val -= board.pieces(chess::PieceType::ROOK, otherCol).count() * PieceValues::ROOK;
		val -= board.pieces(chess::PieceType::KNIGHT, otherCol).count() * PieceValues::KNIGHT;
		val -= board.pieces(chess::PieceType::BISHOP, otherCol).count() * PieceValues::BISHOP;
		val -= board.pieces(chess::PieceType::QUEEN, otherCol).count() * PieceValues::QUEEN;
		val -= board.pieces(chess::PieceType::KING, otherCol).count() * PieceValues::KING;

		//Check number of attacks both side can make
		val -= NumAvailableAttacks(player);
		val += NumAvailableAttacks(otherCol);
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