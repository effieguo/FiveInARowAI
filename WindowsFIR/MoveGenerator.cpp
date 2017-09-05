#include "MoveGenerator.h"
#include "ChessBoard.h"
#include "Evaluation.h"
#include "HistoryHeuristic.h"
#include "ArtIntel.h"
#include <string>
#include <ctime>

ChessMove MoveGenerator::BestMove;

MoveGenerator::MoveGenerator(void)
{
	srand((unsigned)time(NULL));
}

MoveGenerator::~MoveGenerator(void)
{
}

extern GameBoard gGameBoard;

int MoveGenerator::CreatePossibleMove(Chess chs, int depth, Evaluation& Eva)
{
	int MoveCount = 0;
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			if (gGameBoard.boardState[i][j] == NONE)
				AddMove(j, i, chs, depth, Eva, MoveCount);
		}
	}

	memcpy(moveList[depth], tmpMoveBuff, MoveCount * sizeof(ChessMove));
	return MoveCount;
}

void MoveGenerator::AddMove(int x, int y, Chess chs, int depth, Evaluation& Eva, int& MoveCount)
{
	tmpMoveBuff[MoveCount].ChessPos.x = x;
	tmpMoveBuff[MoveCount].ChessPos.y = y;

	MoveCount++;
}

