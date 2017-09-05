#pragma once
#include "Evaluation.h"
#include "FIR.h"

class MoveGenerator
{
public:
	ChessMove moveList[10][225];
	ChessMove tmpMoveBuff[225];
	static ChessMove BestMove;

public:
	int CreatePossibleMove(Chess, int, Evaluation&);
	void AddMove(int, int, Chess, int, Evaluation&, int& MoveCount);
	MoveGenerator(void);
	~MoveGenerator(void);
};

