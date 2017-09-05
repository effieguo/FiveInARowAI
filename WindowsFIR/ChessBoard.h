#pragma once
#include "FIR.h"

class GameBoard
{
public:
	Chess boardState[15][15];
	Chess turn;
	MoveHis moveHistory;
	static const int GridSize = 36;

public:
	GameBoard(void);
	~GameBoard(void);
	void SaveCurMove(const ChessMove&);
	void SaveCurMove(int, int);


	bool IsMoveEmpty(const Coordinate& coord) const;
	void PlaceStone(const Coordinate& coord, const Chess chess)
	{
		boardState[coord.y][coord.x] = chess;
		SaveCurMove(coord.y, coord.x);
	}

	static Coordinate MousePosToMovePos(const int x, const int y);
};



