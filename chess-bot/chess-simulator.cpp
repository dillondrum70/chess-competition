#include "chess-simulator.h"
// disservin's lib. drop a star on his hard work!
// https://github.com/Disservin/chess-library
#include "chess.hpp"

#include "Board.h"
#include "MinMax.h"

#include <random>
using namespace ChessSimulator;

std::string ChessSimulator::Move(std::string fen) {
  // create your board based on the board string following the FEN notation
  // search for the best move using minimax / monte carlo tree search /
  // alpha-beta pruning / ... try to use nice heuristics to speed up the search
  // and have better results return the best move in UCI notation you will gain
  // extra points if you create your own board/move representation instead of
  // using the one provided by the library

  // here goes a random movement
  //chess::Board board(fen);
  //chess::Movelist moves;
  //chess::movegen::legalmoves(moves, board);
  //if(moves.size() == 0)
  //  return "";

  //// get random move
  //std::random_device rd;
  //std::mt19937 gen(rd());
  //std::uniform_int_distribution<> dist(0, moves.size() - 1);
  //auto move = moves[dist(gen)];
  //return chess::uci::moveToUci(move);

	ChessBoard chBoard = ChessBoard(chess::Board(fen));

	////Black moves randomly
	//if (chBoard.board.sideToMove() == chess::Color::BLACK)
	//{
	//	chess::Movelist moves;
	//	chess::movegen::legalmoves(moves, chBoard.board);
	//	if(moves.size() == 0)
	//	  return "";

	//	// get random move
	//	std::random_device rd;
	//	std::mt19937 gen(rd());
	//	std::uniform_int_distribution<> dist(0, moves.size() - 1);
	//	auto mv = moves[dist(gen)];
	//	std::cout << "BLACK: " << chess::uci::moveToUci(mv) << std::endl;
	//	return chess::uci::moveToUci(mv);
	//}

	float score = 0;

	//White uses minmax
	chess::Move move = MinMax::FindBestMove(chBoard, score);
	/*std::cout << (chBoard.board.sideToMove() == chess::Color::WHITE ? "WHITE: " : "BLACK: ") << chess::uci::moveToUci(move) 
		<< "   " << std::to_string(score) << std::endl;*/
	return chess::uci::moveToUci(move);
}