#include "ArtIntel.h"

extern GameBoard gGameBoard;

int ArtificialIntelligence::Depth = 0;

ArtificialIntelligence::ArtificialIntelligence(void)
{
}

ArtificialIntelligence::~ArtificialIntelligence(void)
{
}

void ArtificialIntelligence::AIMakeMove(Chess chs, int depth, int alpha, int beta)
{
	switch (searchMethod)
	{
	case MinMax:
		MinMaxSearch(chs, depth);
		break;
	case AlphaBeta:
		historyHeuristic.ResetHistoryTable();
		AlphaBetaSearch(chs, depth, INT_MIN, INT_MAX);
		break;
	}
}


int ArtificialIntelligence::MinMaxSearch(Chess chs, int depth)
{
	if (depth > 0)
	{
		int Win = IsGameOver(chs);
		if (Win != 0)
			return Win;
	}

	if (depth <= 0)
	{
		return eval.Eval(chs);
	}

	int BestMove = -1;
	int BestScore = ArtificialIntelligence::Infinity;

	int MoveCount = moveGenerator.CreatePossibleMove(chs, depth, eval);

	for (int i = 0; i < MoveCount; i++)
	{
		gGameBoard.boardState[moveGenerator.moveList[depth][i].ChessPos.y][moveGenerator.moveList[depth][i].ChessPos.x] = chs;

		int Score = MinMaxSearch(Chess(!chs), depth - 1);

		if (BestScore > Score)
		{
			BestScore = Score;
			if (depth == ArtificialIntelligence::Depth)
			{
				MoveGenerator::BestMove = moveGenerator.moveList[depth][i];
			}
		}

		gGameBoard.boardState[moveGenerator.moveList[depth][i].ChessPos.y][moveGenerator.moveList[depth][i].ChessPos.x] = NONE;
	}

	return BestScore;
}

int ArtificialIntelligence::AlphaBetaSearch(Chess chs, int depth, int alpha, int beta)
{
	if (depth > 0)
	{
		int Win = IsGameOver(chs);
		if (Win != 0)
			return Win;
	}

	if (depth <= 0)
	{
		return eval.Eval(chs);
	}

	int BestMove = -1;
	int BestScore = chs == BLACK ? INT_MIN : INT_MAX;

	int MoveCount = moveGenerator.CreatePossibleMove(chs, depth, eval);

	// Sort all the possible moves according to history scores in previous search to improve pruning efficiency
	HistoryHeuristic::MergeSort(moveGenerator.moveList[depth], MoveCount, historyHeuristic.historyTable);

	for (int i = 0; i < MoveCount; i++)
	{
		gGameBoard.boardState[moveGenerator.moveList[depth][i].ChessPos.y][moveGenerator.moveList[depth][i].ChessPos.x] = chs;

		int Score = AlphaBetaSearch(Chess(!chs), depth - 1, alpha, beta);

		gGameBoard.boardState[moveGenerator.moveList[depth][i].ChessPos.y][moveGenerator.moveList[depth][i].ChessPos.x] = NONE;

		if (chs == BLACK)
		{
			if (BestScore < Score)
			{
				BestScore = Score;
				if (depth == ArtificialIntelligence::Depth)
				{
					MoveGenerator::BestMove = moveGenerator.moveList[depth][i];
				}
			}
			alpha = max(alpha, BestScore);

		}

		if (chs == WHITE)
		{
			if (BestScore > Score)
			{
				BestScore = Score;
				if (depth == ArtificialIntelligence::Depth)
				{
					MoveGenerator::BestMove = moveGenerator.moveList[depth][i];
				}
			}
			beta = min(beta, BestScore);
		}

		if (beta <= alpha)
			break;
	}

	if (BestMove != -1)
		historyHeuristic.EnterHistoryTable(moveGenerator.moveList[depth][BestMove], depth);

	return BestScore;

}

int ArtificialIntelligence::IsGameOver(Chess chs)
{
	int score = eval.Eval(chs);
	if (score > 300000 || score < -300000)
		return score;
	else
		return 0;
}