#pragma once

#include "MoveGenerator.h"

class HistoryHeuristic
{
public:
	int historyTable[15][15];
	static int ScorePattern[15][15];
public:
	void ResetHistoryTable();
	void EnterHistoryTable(ChessMove, int);
	static void Merge(ChessMove* , ChessMove* , int, int, int, int[][15]);
	static void MergePass(ChessMove* , ChessMove* , int, int, int[][15]);
	static void MergeSort(ChessMove* , int, int[][15]);
	HistoryHeuristic(void);
	~HistoryHeuristic(void);
};
