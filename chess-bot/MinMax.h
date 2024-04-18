#pragma once

#include <stack>
#include <thread>
#include <chrono>
#include <unordered_map>
#include <random>
#include <algorithm>
#include "Board.h"

#include "chess.hpp"

static const int LARGE_NUM = 10000000;
static const int MAX_NUM_BYTES = 134217728;
static const float MAX_SEARCH_TIME = 5.0;

class MinMax
{
public:
	static float MiniMax(ChessBoard board, bool maximizing, chess::Color originalPlayer, int maxDepth = 8)
	{
		// best case reached
		if (board.IsWin() || board.IsDraw() || maxDepth == 0)
			return board.Evaluate(originalPlayer);

		float result;
		chess::Movelist moves = board.LegalMoves();

		// recursive case - maximize your gains or minimize the opponent's gains
		if (maximizing)
		{
			float bestEval = -LARGE_NUM; // arbitrarily low starting point
			

			for (int i = 0; i < moves.size(); i++)
			{
				result = MiniMax(board.MakeMove(moves[i]), false, originalPlayer, maxDepth - 1);

				if (result > bestEval)
					bestEval = result;
			}
			return bestEval;
		}
		else
		{
			float worstEval = LARGE_NUM;
			for (int i = 0; i < moves.size(); i++)
			{
				result = MiniMax(board.MakeMove(moves[i]), true, originalPlayer, maxDepth - 1);

				if (result < worstEval)
					worstEval = result;
			}
			return worstEval;
		}
	}

	static float AlphaBeta(ChessBoard board, bool maximizing, chess::Color originalPlayer, int maxDepth = 8, float alpha = -LARGE_NUM, float beta = LARGE_NUM)
	{
		// best case reached
		if (board.IsWin() || board.IsDraw() || maxDepth == 0)
			return board.Evaluate(originalPlayer);

		float result;
		chess::Movelist moves = board.LegalMoves();

		// recursive case - maximize your gains or minimize the opponent's gains
		if (maximizing)
		{
			for (int i = 0; i < moves.size(); i++)
			{
				result = AlphaBeta(board.MakeMove(moves[i]), false, originalPlayer, maxDepth - 1, alpha, beta);

				if (result > alpha)
					alpha = result;

				if (beta <= alpha)
					break;
			}
			return alpha;
		}
		else
		{
			for (int i = 0; i < moves.size(); i++)
			{
				result = AlphaBeta(board.MakeMove(moves[i]), true, originalPlayer, maxDepth - 1, alpha, beta);

				if (result < beta)
					beta = result;

				if (beta <= alpha)
					break;
			}
			return beta;
		}
	}

	static void AlphaBetaStack(ChessBoard board, bool maximizing, chess::Color originalPlayer, float& result, float alpha = -LARGE_NUM, float beta = LARGE_NUM)
	{
		// best case reached
		if (board.IsWin() || board.IsDraw())
		{
			result = board.Evaluate(originalPlayer);
			return;
		}
			

		std::unordered_map<ChessBoard, bool> visited;
		std::stack<ChessBoard> boardStack;
		boardStack.push(board);
		visited.emplace(board.board, true);
		ChessBoard currentBoard, tmpBoard;

		chess::Movelist moves;// = board.LegalMoves();

		std::chrono::high_resolution_clock::time_point startTime = std::chrono::high_resolution_clock::now();

		while (!boardStack.empty() || sizeof(boardStack) + (boardStack.size() * sizeof(ChessBoard)) > MAX_NUM_BYTES)
		{
			if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - startTime).count() >= MAX_SEARCH_TIME * 1000)
				break;

			currentBoard = boardStack.top();
			boardStack.pop();
			moves = currentBoard.LegalMoves();

			// recursive case - maximize your gains or minimize the opponent's gains
			if (maximizing)
			{
				for (int i = 0; i < moves.size(); i++)
				{
					tmpBoard = currentBoard.MakeMove(moves[i]);

					if (!visited.contains(tmpBoard.board))
					{
						visited.emplace(tmpBoard.board, true);
						boardStack.push(tmpBoard);
						result = tmpBoard.Evaluate(originalPlayer);
						//result = AlphaBeta(board.MakeMove(moves[i]), false, originalPlayer, maxDepth - 1, alpha, beta);

						if (result > alpha)
							alpha = result;

						if (beta <= alpha)
							break;
					}
				}
				//return alpha;
			}
			else
			{
				for (int i = 0; i < moves.size(); i++)
				{
					tmpBoard = currentBoard.MakeMove(moves[i]);

					if (!visited.contains(tmpBoard.board))
					{
						visited.emplace(tmpBoard.board, true);
						boardStack.push(tmpBoard);
						result = tmpBoard.Evaluate(originalPlayer);
						//result = AlphaBeta(board.MakeMove(moves[i]), true, originalPlayer, maxDepth - 1, alpha, beta);

						if (result < beta)
							beta = result;

						if (beta <= alpha)
							break;
					}
				}
				//return beta;
			}
		}

		if (maximizing)
		{
			result = alpha;
			return;
		}
		else
		{
			result = beta;
			return;
		}
	}

	static void AlphaBetaStackThread(ChessBoard board, bool maximizing, chess::Color originalPlayer, float& result, float alpha, float beta)
	{
		// best case reached
		if (board.IsWin() || board.IsDraw())
		{
			result = board.Evaluate(originalPlayer);
			return;
		}

		std::unordered_map<ChessBoard, bool> visited;
		std::stack<ChessBoard> boardStack;
		boardStack.push(board);
		visited.emplace(board.board, true);
		ChessBoard currentBoard, tmpBoard;

		float score;

		chess::Movelist moves;// = board.LegalMoves();

		std::chrono::high_resolution_clock::time_point startTime = std::chrono::high_resolution_clock::now();

		while (!boardStack.empty() || sizeof(boardStack) + (boardStack.size() * sizeof(ChessBoard)) > MAX_NUM_BYTES)
		{
			if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - startTime).count() >= MAX_SEARCH_TIME * 1000)
				break;

			currentBoard = boardStack.top();
			boardStack.pop();
			moves = currentBoard.LegalMoves();

			// recursive case - maximize your gains or minimize the opponent's gains
			if (maximizing)
			{
				for (int i = 0; i < moves.size(); i++)
				{
					tmpBoard = currentBoard.MakeMove(moves[i]);

					if (!visited.contains(tmpBoard.board))
					{
						visited.emplace(tmpBoard.board, true);
						boardStack.push(tmpBoard);
						score = tmpBoard.Evaluate(originalPlayer);

						if (score > alpha)
							alpha = score;

						if (beta <= alpha)
							break;
					}
				}
			}
			else
			{
				for (int i = 0; i < moves.size(); i++)
				{
					tmpBoard = currentBoard.MakeMove(moves[i]);

					if (!visited.contains(tmpBoard.board))
					{
						visited.emplace(tmpBoard.board, true);
						boardStack.push(tmpBoard);
						score = tmpBoard.Evaluate(originalPlayer);

						if (score < beta)
							beta = score;

						if (beta <= alpha)
							break;
					}
				}
			}

			originalPlayer = !originalPlayer;
		}

		if (maximizing)
			result = alpha;
		else
			result = beta;
	}

	static chess::Move FindBestMove(ChessBoard board, float& score)
	{
		float bestEval = -LARGE_NUM;
		chess::Move bestMove;

		chess::Movelist moves = board.LegalMoves();
		std::vector<float> results;
		std::vector<std::thread> threads;

		// Shuffle the valid moves
		std::random_device rd;
		std::mt19937 g(rd());
		std::shuffle(moves.begin(), moves.end(), g);

		// Fill results array now so vector location is not moved when new data is pushed back and pointers are incorrect
		for (int i = 0; i < moves.size(); i++)
			results.push_back(0.0);

		// Start all threads and place them in the vector
		for (int i = 0; i < moves.size(); i++)
		{
			threads.emplace_back(std::thread(&MinMax::AlphaBetaStack, board.MakeMove(moves[i]), false, board.board.sideToMove(), std::ref(results[i]), -LARGE_NUM, LARGE_NUM));
		}

		for (int i = 0; i < threads.size(); i++)
			threads[i].join();

		for (int i = 0; i < results.size(); i++)
		{
			if (results[i] > bestEval)
			{
				bestEval = results[i];
				bestMove = moves[i];
			}
		}

		score = bestEval;
		return bestMove;
	}
};