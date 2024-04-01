#pragma once

#include <stack>
#include <thread>
#include <chrono>
#include <unordered_map>
#include <random>
#include <algorithm>
#include "Board.h"

static const int LARGE_NUM = 10000000;
static const int MAX_NUM_BYTES = 134217728;
static const float MAX_SEARCH_TIME = 5.0;

template <typename board_t, typename move_t>
	requires std::is_same<board_t, NTTTBoard>::value && std::is_same<move_t, NTTTMove>::value
class MinMax
{
public:
	//template <typename T>
	static float MiniMax(board_t board, bool maximizing, bool originalPlayer, int maxDepth = 8)
	{
		// best case reached
		if (board.IsWin() || board.IsDraw() || maxDepth == 0)
			return board.Evaluate(originalPlayer);

		float result;
		std::vector<move_t> moves = board.LegalMoves();

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

	static float AlphaBeta(board_t board, bool maximizing, bool originalPlayer, int maxDepth = 8, float alpha = -LARGE_NUM, float beta = LARGE_NUM)
	{
		// best case reached
		if (board.IsWin() || board.IsDraw() || maxDepth == 0)
			return board.Evaluate(originalPlayer);

		float result;
		std::vector<move_t> moves = board.LegalMoves();

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

	static float AlphaBetaStack(board_t board, bool maximizing, bool originalPlayer, float alpha = -LARGE_NUM, float beta = LARGE_NUM)
	{
		// best case reached
		if (board.IsWin() || board.IsDraw())
			return board.Evaluate(originalPlayer);

		std::unordered_map<NTTT, bool> visited;
		std::stack<board_t> boardStack;
		boardStack.push(board);
		visited.emplace(board.board, true);
		board_t currentBoard, tmpBoard;

		float result;

		std::vector<NTTTMove> moves;// = board.LegalMoves();

		std::chrono::high_resolution_clock::time_point startTime = std::chrono::high_resolution_clock::now();

		while (!boardStack.empty() || sizeof(boardStack) + (boardStack.size() * sizeof(board_t)) > MAX_NUM_BYTES)
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
			return alpha;
		else
			return beta;
	}

	static void AlphaBetaStackThread(board_t board, bool maximizing, bool originalPlayer, float& result, float alpha, float beta)
	{
		// best case reached
		if (board.IsWin() || board.IsDraw())
		{
			result = board.Evaluate(originalPlayer);
			return;
		}

		std::unordered_map<NTTT, bool> visited;
		std::stack<board_t> boardStack;
		boardStack.push(board);
		visited.emplace(board.board, true);
		board_t currentBoard, tmpBoard;

		float score;

		std::vector<NTTTMove> moves;// = board.LegalMoves();

		std::chrono::high_resolution_clock::time_point startTime = std::chrono::high_resolution_clock::now();

		while (!boardStack.empty() || sizeof(boardStack) + (boardStack.size() * sizeof(board_t)) > MAX_NUM_BYTES)
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

	static move_t FindBestMove(board_t board)
	{
		float bestEval = -LARGE_NUM;
		move_t bestMove;

		std::vector<move_t> moves = board.LegalMoves();
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
			threads.emplace_back(std::thread(&MinMax::AlphaBetaStackThread, board.MakeMove(moves[i]), false, board.player, std::ref(results[i]), -LARGE_NUM, LARGE_NUM));

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

		return bestMove;
	}
};