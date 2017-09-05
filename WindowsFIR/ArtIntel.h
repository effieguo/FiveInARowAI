#pragma once
#include "HistoryHeuristic.h"
#include "Evaluation.h"
#include "FIR.h"
#include <Windows.h>

class ArtificialIntelligence
{
public:
	const static int Infinity = INT_MAX;
	static int Depth;
	Evaluation eval;

	enum SearchMethod
	{
		MinMax, AlphaBeta
	};

	SearchMethod searchMethod;

private:
	MoveGenerator moveGenerator;
	HistoryHeuristic historyHeuristic;

public:
	ArtificialIntelligence(void);
	~ArtificialIntelligence(void);
	void AIMakeMove(Chess, int, int, int);
	int IsGameOver(Chess);

private:
	int ArtificialIntelligence::AlphaBetaSearch(Chess chs, int depth, int alpha, int beta);
	int MinMaxSearch(Chess chs, int depth);
};
