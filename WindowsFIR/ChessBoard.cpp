#include "ChessBoard.h"

GameBoard::GameBoard(void)
{
}

GameBoard::~GameBoard(void)
{
}

void GameBoard::SaveCurMove(const ChessMove& CM)
{
	moveHistory.Count++;
	moveHistory.Move[moveHistory.Count] = CM;
}

void GameBoard::SaveCurMove(int y, int x)
{
	moveHistory.Count++;
	moveHistory.Move[moveHistory.Count].ChessPos.y = y;
	moveHistory.Move[moveHistory.Count].ChessPos.x = x;
}

Coordinate GameBoard::MousePosToMovePos(const int x, const int y)
{
	// Calculate move position based on mouse position
	Coordinate ret = { x / GridSize, y / GridSize };

	return ret;
}

bool GameBoard::IsMoveEmpty(const Coordinate& coord) const
{
	return boardState[coord.y][coord.x] == NONE;
}