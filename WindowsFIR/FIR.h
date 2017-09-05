#pragma once

enum Chess { BLACK = 0, WHITE , NONE };

struct Coordinate
{
	int x;
	int y;
};

struct ChessMove
{
	Coordinate ChessPos;
	int Value;
};

struct MoveHis
{
	ChessMove Move[225];
	int Count;
	MoveHis()
	{
		Count = -1;
	}
};