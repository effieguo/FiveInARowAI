#pragma once
#include "ChessBoard.h"
#include "FIR.h"

class Evaluation
{
private:
	enum EvaType
	{
		SleepTwo, FreeTwo, SleepThree, FreeThree, SleepFour, FreeFour, Five, Analysised, UnAnalysised
	};
	int Score;
	EvaType LineRecord[15];
	EvaType TypeRecord[15][15][4];
	Chess TempLine[15];
	int TotalCount[2][7];
	EvaType InitType[15][15][4];
	EvaType InitLine[15];
	int InitCount[2][7];
public:
	Evaluation(void);
	~Evaluation(void);
	int Eval(Chess);
	static int Count;
private:
	void AnalysisHorizon(int y, int x);
	void AnalysisVertical(int y, int x);
	void AnalysisLeft(int y, int x);
	void AnalysisRight(int y, int x);
	void AnalysisLine(Chess AnaLine[], int Length, int ChessPos);
};
