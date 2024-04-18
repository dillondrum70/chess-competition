#pragma once
#include <vector>
#include <iostream>

#include "chess.hpp"

//std::vector<int> evens = { 2, 4, 6, 8 };
//std::vector<int> odds = { 1, 3, 5, 7, 9 };

/////// WEIGHTS
const float MATERIAL_ADVANTAGE_WEIGHT = 1.f;	//Weight applied to part of score tracking how many pieces are on the board
const float ATTACKING_WEIGHT = .5f;				//Weight applied to part of score tracking how many spaces are attacked by each side


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

		int numAttacks = 0, boardPos = 0;
		for (int i = 0; boardPos < 64; i++)
		{
			//ch = fen[i];
			if (board.isAttacked(chess::Square(i), player))
			{
				numAttacks += 1;
			}
			
			boardPos++;
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
		val = ((PieceValues::KING * (board.pieces(chess::PieceType::KING, player).count() - board.pieces(chess::PieceType::KING, otherCol).count())) +
			(PieceValues::QUEEN * (board.pieces(chess::PieceType::QUEEN, player).count() - board.pieces(chess::PieceType::QUEEN, otherCol).count())) +
			(PieceValues::ROOK * (board.pieces(chess::PieceType::ROOK, player).count() - board.pieces(chess::PieceType::ROOK, otherCol).count())) +
			(PieceValues::BISHOP * (board.pieces(chess::PieceType::BISHOP, player).count() - board.pieces(chess::PieceType::BISHOP, otherCol).count())) +
			(PieceValues::KNIGHT * (board.pieces(chess::PieceType::KNIGHT, player).count() - board.pieces(chess::PieceType::KNIGHT, otherCol).count())) +
			(PieceValues::PAWN * (board.pieces(chess::PieceType::PAWN, player).count() - board.pieces(chess::PieceType::PAWN, otherCol).count())) +
			(0.1f * LegalMoves().size())) * MATERIAL_ADVANTAGE_WEIGHT;

		//Check number of attacks both side can make
		int ourAttacks = NumAvailableAttacks(player);
		int opponentAttacks = NumAvailableAttacks(otherCol);
		//std::cout << num1 << "   " << num2 << std::endl;
		val += (ourAttacks - opponentAttacks) * ATTACKING_WEIGHT;

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